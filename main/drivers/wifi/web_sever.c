//
// Created by k0921 on 2026/3/20.
//

#include "web_sever.h"
#include "cJSON.h"
#include "led/light_custom.h"
#include "lcd/ui_manager/time_bg.h"
#include "esp_log.h"
#include <esp_err.h>
#include <esp_http_server.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAG "web_sever"

//网页
static const char* http_html = NULL;

//websocket接收数据回调函数
static ws_receive_cb ws_receive_fn = NULL;

static httpd_handle_t sever_handle;

static int client_fds = -1;

static const char *photo_html =
    "<!doctype html><html><head><meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
    "<title>White Knob 设置</title>"
    "<style>"
    ":root{color-scheme:dark;--bg:#08090d;--panel:rgba(18,22,32,.78);--line:rgba(255,255,255,.12);--text:#f6f7fb;--muted:#9aa4b2;--accent:#65d6ad;--accent2:#7aa8ff;--danger:#ff8585}"
    "*{box-sizing:border-box;-webkit-tap-highlight-color:transparent}"
    "body{margin:0;min-height:100vh;color:var(--text);font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Arial,sans-serif;background:radial-gradient(circle at 20% 0,#1c3446 0,transparent 34%),radial-gradient(circle at 90% 15%,#2a1f56 0,transparent 30%),linear-gradient(145deg,#07080d,#111827 55%,#06130f);}"
    "body:before{content:'';position:fixed;inset:0;background:linear-gradient(120deg,rgba(255,255,255,.08),transparent 28%,rgba(255,255,255,.04));pointer-events:none}"
    "main{width:min(430px,100%);margin:0 auto;padding:24px 16px 34px;position:relative;animation:rise .42s ease-out}"
    "@keyframes rise{from{opacity:0;transform:translateY(10px)}to{opacity:1;transform:none}}"
    ".hero{padding:8px 4px 18px}.eyebrow{font-size:12px;color:var(--accent);letter-spacing:.08em;text-transform:uppercase}.hero h1{font-size:30px;line-height:1.08;margin:8px 0 8px}.hero p{margin:0;color:var(--muted);min-height:0}"
    ".panel{background:var(--panel);border:1px solid var(--line);border-radius:22px;padding:18px;margin:14px 0;box-shadow:0 22px 60px rgba(0,0,0,.35);backdrop-filter:blur(18px)}"
    ".head{display:flex;justify-content:space-between;gap:12px;align-items:flex-start;margin-bottom:14px}.head h2{font-size:19px;margin:0}.tag{font-size:12px;color:#c8d1dc;border:1px solid var(--line);border-radius:999px;padding:5px 9px;background:rgba(255,255,255,.06)}"
    "canvas{display:block;width:240px;height:240px;margin:16px auto;background:#000;border-radius:50%;border:1px solid rgba(255,255,255,.18);box-shadow:0 16px 45px rgba(0,0,0,.42)}"
    "input,select,button{box-sizing:border-box;width:100%;font-size:16px;margin:7px 0;padding:13px 14px;border-radius:14px;border:1px solid var(--line);font-family:inherit}"
    "input,select{background:rgba(255,255,255,.08);color:var(--text);outline:none}select option{background:#111827;color:#fff}input[type=file]{color:var(--muted)}input[type=file]::file-selector-button{border:0;border-radius:11px;background:#eef2ff;color:#111827;padding:10px 12px;margin-right:10px}"
    "input[type=color]{height:48px;padding:5px}input[type=range]{accent-color:var(--accent);padding:0;border:0;background:transparent}"
    "label{display:block;margin-top:11px;color:#dbe2ea;font-size:14px}.row{display:grid;grid-template-columns:1fr 1fr;gap:11px}.check{display:flex;align-items:center;gap:10px;margin:8px 0 12px}.check input{width:22px;height:22px;margin:0;accent-color:var(--accent)}"
    "button{border:0;background:linear-gradient(135deg,var(--accent),#3fbf98);color:#06130f;font-weight:750;box-shadow:0 10px 24px rgba(101,214,173,.22);cursor:pointer;transition:transform .16s ease,opacity .16s ease}"
    "button.secondary{background:linear-gradient(135deg,var(--accent2),#8f7dff);color:#fff;box-shadow:0 10px 24px rgba(122,168,255,.22)}button:active{transform:scale(.985)}button:disabled{opacity:.52;transform:none;cursor:wait}"
    ".status{min-height:24px;margin:9px 0 0;color:#c6f6d5;font-size:14px;word-break:break-word}.actions{display:grid;grid-template-columns:1fr 1fr;gap:11px;margin-top:10px}"
    ".link{display:block;text-align:center;color:#a8c7ff;text-decoration:none;margin-top:16px;font-size:14px}.link:active{opacity:.75}"
    "@media(max-width:380px){main{padding:18px 12px 28px}.hero h1{font-size:26px}.panel{padding:15px;border-radius:19px}.row,.actions{grid-template-columns:1fr}canvas{width:220px;height:220px}}"
    "@media(prefers-reduced-motion:reduce){main{animation:none}button{transition:none}}"
    "</style></head><body><main>"
    "<div class=\"hero\"><div class=\"eyebrow\">White Knob Studio</div><h1>旋钮设置中心</h1><p>照片上传与灯光自定义</p></div>"
    "<section class=\"panel\"><div class=\"head\"><h2>时间界面照片</h2><span class=\"tag\">240 x 240</span></div>"
    "<input id=\"file\" type=\"file\" accept=\"image/*\">"
    "<canvas id=\"preview\" width=\"240\" height=\"240\"></canvas>"
    "<button id=\"upload\">上传照片</button>"
    "<p id=\"msg\" class=\"status\"></p></section>"
    "<section class=\"panel\"><div class=\"head\"><h2>灯光自定义</h2><span class=\"tag\">WS2812</span></div>"
    "<label class=\"check\"><input id=\"lightOn\" type=\"checkbox\">启用灯光</label>"
    "<label>模式<select id=\"lightMode\">"
    "<option value=\"1\">纯色</option>"
    "<option value=\"4\">呼吸</option>"
    "<option value=\"5\">流星</option>"
    "<option value=\"2\">渐变</option>"
    "<option value=\"0\">彩虹</option>"
    "</select></label>"
    "<div class=\"row\"><label>颜色<input id=\"lightColor\" type=\"color\" value=\"#2ec5ce\"></label>"
    "<label>方向<select id=\"lightDir\"><option value=\"0\">正向</option><option value=\"1\">反向</option></select></label></div>"
    "<label>亮度 <span id=\"brText\">255</span><input id=\"lightBrightness\" type=\"range\" min=\"0\" max=\"255\" value=\"255\"></label>"
    "<label>速度 <span id=\"spText\">10</span><input id=\"lightSpeed\" type=\"range\" min=\"0\" max=\"100\" value=\"10\"></label>"
    "<div class=\"actions\"><button id=\"lightPreview\" class=\"secondary\">预览</button><button id=\"lightSave\">保存</button></div>"
    "<p id=\"lightMsg\" class=\"status\"></p><a class=\"link\" href=\"/\">返回配网页面</a></section>"
    "</main><script>"
    "const W=240,H=240,SIZE=W*H*2;"
    "const file=document.getElementById('file');"
    "const canvas=document.getElementById('preview');"
    "const ctx=canvas.getContext('2d');"
    "const upload=document.getElementById('upload');"
    "const msg=document.getElementById('msg');"
    "const lightOn=document.getElementById('lightOn'),lightMode=document.getElementById('lightMode'),lightColor=document.getElementById('lightColor'),lightDir=document.getElementById('lightDir');"
    "const lightBrightness=document.getElementById('lightBrightness'),lightSpeed=document.getElementById('lightSpeed'),brText=document.getElementById('brText'),spText=document.getElementById('spText');"
    "const lightPreview=document.getElementById('lightPreview'),lightSave=document.getElementById('lightSave'),lightMsg=document.getElementById('lightMsg');"
    "let bin=null;"
    "function say(t){msg.textContent=t;}"
    "function lightSay(t){lightMsg.textContent=t;}"
    "function clamp(v,min,max){v=Number(v);return Math.max(min,Math.min(max,isNaN(v)?min:v));}"
    "function hex2rgb(hex){const n=parseInt(hex.slice(1),16);return{r:(n>>16)&255,g:(n>>8)&255,b:n&255};}"
    "function rgb2hex(r,g,b){return'#'+[r,g,b].map(v=>clamp(v,0,255).toString(16).padStart(2,'0')).join('');}"
    "function updateLightText(){brText.textContent=lightBrightness.value;spText.textContent=lightSpeed.value;}"
    "function lightPayload(){const c=hex2rgb(lightColor.value);return{enabled:lightOn.checked,mode:+lightMode.value,direction:+lightDir.value,brightness:+lightBrightness.value,speed:+lightSpeed.value,r:c.r,g:c.g,b:c.b};}"
    "async function sendLight(path){lightPreview.disabled=true;lightSave.disabled=true;lightSay(path.includes('preview')?'预览中...':'保存中...');try{const res=await fetch(path,{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(lightPayload())});const text=await res.text();lightSay(res.ok?(path.includes('preview')?'已预览':'已保存'):text);}catch(e){lightSay('灯光请求失败');}lightPreview.disabled=false;lightSave.disabled=false;}"
    "async function loadLight(){try{const res=await fetch('/light_config');if(!res.ok)return;const c=await res.json();lightOn.checked=!!c.enabled;lightMode.value=String(c.mode);lightDir.value=String(c.direction);lightBrightness.value=c.brightness;lightSpeed.value=c.speed;lightColor.value=rgb2hex(c.r,c.g,c.b);updateLightText();}catch(e){}}"
    "function convert(){"
    "const d=ctx.getImageData(0,0,W,H).data;"
    "const out=new Uint8Array(SIZE);"
    "for(let i=0,j=0;i<d.length;i+=4){"
    "const a=d[i+3]/255;"
    "const r=Math.round(d[i]*a);"
    "const g=Math.round(d[i+1]*a);"
    "const b=Math.round(d[i+2]*a);"
    "const c=((r&248)<<8)|((g&252)<<3)|(b>>3);"
    "out[j++]=c>>8;"
    "out[j++]=c&255;"
    "}"
    "bin=out;"
    "say('已就绪：'+SIZE+' 字节 RGB565');"
    "}"
    "file.onchange=()=>{"
    "const f=file.files[0];"
    "if(!f)return;"
    "const url=URL.createObjectURL(f);"
    "const img=new Image();"
    "img.onload=()=>{"
    "ctx.fillStyle='#000';"
    "ctx.fillRect(0,0,W,H);"
    "const scale=Math.max(W/img.width,H/img.height);"
    "const sw=W/scale,sh=H/scale;"
    "const sx=(img.width-sw)/2,sy=(img.height-sh)/2;"
    "ctx.drawImage(img,sx,sy,sw,sh,0,0,W,H);"
    "URL.revokeObjectURL(url);"
    "convert();"
    "};"
    "img.onerror=()=>{URL.revokeObjectURL(url);say('图片加载失败');};"
    "img.src=url;"
    "};"
    "upload.onclick=async()=>{"
    "if(!bin){say('请先选择图片');return;}"
    "upload.disabled=true;"
    "say('上传中...');"
    "try{"
    "const res=await fetch('/upload_time_bg',{method:'POST',headers:{'Content-Type':'application/octet-stream'},body:bin});"
    "const text=await res.text();"
    "say(res.ok?'已保存':text);"
    "}catch(e){say('上传失败');}"
    "upload.disabled=false;"
    "};"
    "lightBrightness.oninput=updateLightText;"
    "lightSpeed.oninput=updateLightText;"
    "lightPreview.onclick=()=>sendLight('/light_preview');"
    "lightSave.onclick=()=>sendLight('/light_config');"
    "updateLightText();loadLight();"
    "</script></body></html>";

esp_err_t get_http_req(httpd_req_t *req) {
    if (http_html == NULL) {
        return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "网页未加载");
    }
    return httpd_resp_send(req, http_html, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t get_photo_req(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html; charset=utf-8");
    return httpd_resp_send(req, photo_html, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t post_time_bg_req(httpd_req_t *req)
{
    if (req->content_len != TIME_BG_IMAGE_SIZE) {
        ESP_LOGW(TAG, "bad time bg upload size: %u", (unsigned)req->content_len);
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "图片尺寸不正确");
    }

    uint8_t *image = (uint8_t *)malloc(TIME_BG_IMAGE_SIZE);
    if (image == NULL) {
        ESP_LOGE(TAG, "malloc upload buffer failed");
        return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "内存不足");
    }

    size_t received = 0;
    int timeout_count = 0;
    while (received < TIME_BG_IMAGE_SIZE) {
        size_t remain = TIME_BG_IMAGE_SIZE - received;
        size_t to_read = remain > 2048 ? 2048 : remain;
        int ret = httpd_req_recv(req, (char *)image + received, to_read);
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            if (++timeout_count > 10) {
                free(image);
                ESP_LOGE(TAG, "receive time bg timeout");
                return httpd_resp_send_err(req, HTTPD_408_REQ_TIMEOUT, "上传超时");
            }
            continue;
        }
        if (ret <= 0) {
            free(image);
            ESP_LOGE(TAG, "receive time bg failed: %d", ret);
            return ESP_FAIL;
        }
        timeout_count = 0;
        received += ret;
    }

    esp_err_t err = time_bg_save_rgb565(image, TIME_BG_IMAGE_SIZE);
    free(image);
    if (err != ESP_OK) {
        char msg[64];
        snprintf(msg, sizeof(msg), "保存失败: %s", esp_err_to_name(err));
        return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, msg);
    }

    time_bg_refresh_async();
    httpd_resp_set_type(req, "text/plain");
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}

static esp_err_t read_small_body(httpd_req_t *req, char *buf, size_t buf_size)
{
    if (buf == NULL || buf_size == 0 || req->content_len >= buf_size) {
        return ESP_ERR_INVALID_SIZE;
    }

    size_t received = 0;
    int timeout_count = 0;
    while (received < req->content_len) {
        int ret = httpd_req_recv(req, buf + received, req->content_len - received);
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            if (++timeout_count > 5) {
                return ESP_ERR_TIMEOUT;
            }
            continue;
        }
        if (ret <= 0) {
            return ESP_FAIL;
        }
        timeout_count = 0;
        received += ret;
    }
    buf[received] = '\0';
    return ESP_OK;
}

static bool json_get_u8(cJSON *root, const char *key, uint8_t min, uint8_t max, uint8_t *out)
{
    cJSON *item = cJSON_GetObjectItem(root, key);
    if (!cJSON_IsNumber(item)) {
        return false;
    }
    int value = item->valueint;
    if (value < min || value > max) {
        return false;
    }
    *out = (uint8_t)value;
    return true;
}

static esp_err_t parse_light_config_json(const char *json, light_custom_config_t *cfg)
{
    if (json == NULL || cfg == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    light_custom_config_t next;
    esp_err_t err = light_custom_load(&next);
    if (err != ESP_OK) {
        light_custom_get_current(&next);
    }

    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    cJSON *enabled = cJSON_GetObjectItem(root, "enabled");
    if (cJSON_IsBool(enabled)) {
        next.enabled = cJSON_IsTrue(enabled);
    }

    uint8_t value;
    if (json_get_u8(root, "mode", 0, MODE_COMET, &value)) {
        next.mode = (led_control_mode)value;
    }
    if (json_get_u8(root, "direction", LED_FORWARD, LED_BACKWARD, &value)) {
        next.direction = (led_direction)value;
    }
    if (json_get_u8(root, "brightness", 0, 255, &value)) {
        next.brightness = value;
    }
    if (json_get_u8(root, "speed", 0, WS2812_MAX_SPEED, &value)) {
        next.speed = value;
    }
    if (json_get_u8(root, "r", 0, 255, &value)) {
        next.rgb_color[R_COLOR] = value;
    }
    if (json_get_u8(root, "g", 0, 255, &value)) {
        next.rgb_color[G_COLOR] = value;
    }
    if (json_get_u8(root, "b", 0, 255, &value)) {
        next.rgb_color[B_COLOR] = value;
    }

    cJSON_Delete(root);

    err = light_custom_validate(&next);
    if (err != ESP_OK) {
        return err;
    }

    *cfg = next;
    return ESP_OK;
}

static esp_err_t send_light_config_json(httpd_req_t *req, const light_custom_config_t *cfg)
{
    char json[160];
    int len = snprintf(json, sizeof(json),
                       "{\"enabled\":%s,\"mode\":%u,\"direction\":%u,\"brightness\":%u,\"speed\":%u,\"r\":%u,\"g\":%u,\"b\":%u}",
                       cfg->enabled ? "true" : "false",
                       (unsigned)cfg->mode,
                       (unsigned)cfg->direction,
                       (unsigned)cfg->brightness,
                       (unsigned)cfg->speed,
                       (unsigned)cfg->rgb_color[R_COLOR],
                       (unsigned)cfg->rgb_color[G_COLOR],
                       (unsigned)cfg->rgb_color[B_COLOR]);
    if (len <= 0 || len >= (int)sizeof(json)) {
        return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "配置数据过长");
    }

    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, json, len);
}

static esp_err_t get_light_config_req(httpd_req_t *req)
{
    light_custom_config_t cfg;
    esp_err_t err = light_custom_load(&cfg);
    if (err != ESP_OK) {
        light_custom_get_current(&cfg);
    }
    return send_light_config_json(req, &cfg);
}

static esp_err_t post_light_common_req(httpd_req_t *req, bool save)
{
    char json[256];
    esp_err_t err = read_small_body(req, json, sizeof(json));
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "read light config failed: %s", esp_err_to_name(err));
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "灯光请求无效");
    }

    light_custom_config_t cfg;
    err = parse_light_config_json(json, &cfg);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "parse light config failed: %s", esp_err_to_name(err));
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "灯光配置无效");
    }

    err = light_custom_apply(&cfg);
    if (err != ESP_OK) {
        return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "应用灯光失败");
    }

    if (save) {
        err = light_custom_save(&cfg);
        if (err != ESP_OK) {
            return httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "保存灯光失败");
        }
    }

    httpd_resp_set_type(req, "text/plain");
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}

static esp_err_t post_light_preview_req(httpd_req_t *req)
{
    return post_light_common_req(req, false);
}

static esp_err_t post_light_config_req(httpd_req_t *req)
{
    return post_light_common_req(req, true);
}

esp_err_t handle_ws_req(httpd_req_t *req) {
    if (req->method == HTTP_GET) {
        client_fds = httpd_req_to_sockfd(req);
        return ESP_OK;
    }

    httpd_ws_frame_t pkt;
    esp_err_t ret;
    memset(&pkt, 0, sizeof(pkt));
    ret = httpd_ws_recv_frame(req, &pkt, 0);
    if (ret != ESP_OK) {
        return ret;
    }
    uint8_t *buf = (uint8_t *)malloc(pkt.len + 1);
    if (buf == NULL) {
        return ESP_FAIL;
    }
    pkt.payload = buf;
    ret = httpd_ws_recv_frame(req, &pkt, pkt.len);
    buf[pkt.len] = '\0';
    if (ret == ESP_OK) {
        if (pkt.type == HTTPD_WS_TYPE_TEXT) {
            ESP_LOGI(TAG, "Get websocket message:%s", pkt.payload);
            if (ws_receive_fn) {
                ws_receive_fn(pkt.payload, pkt.len);
            }
        }
    }
    free(buf);
    return ret;

}

esp_err_t web_ws_start(ws_cfg_t * cfg) {
    if (cfg == NULL) {
        return ESP_FAIL;
    }
    if (cfg->html_code == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (sever_handle != NULL) {
        web_ws_stop();
    }
    http_html = cfg->html_code;
    ws_receive_fn = cfg->receive_fn;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.recv_wait_timeout = 10;
    esp_err_t ret = httpd_start(&sever_handle, &config);
    if (ret != ESP_OK) {
        sever_handle = NULL;
        return ret;
    }
    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_http_req,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_get);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    httpd_uri_t uri_ws = {
        .uri = "/ws",
        .method = HTTP_GET,
        .handler = handle_ws_req,
        .is_websocket = true,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_ws);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    httpd_uri_t uri_photo = {
        .uri = "/photo",
        .method = HTTP_GET,
        .handler = get_photo_req,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_photo);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    httpd_uri_t uri_upload_time_bg = {
        .uri = "/upload_time_bg",
        .method = HTTP_POST,
        .handler = post_time_bg_req,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_upload_time_bg);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    httpd_uri_t uri_get_light_config = {
        .uri = "/light_config",
        .method = HTTP_GET,
        .handler = get_light_config_req,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_get_light_config);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    httpd_uri_t uri_post_light_preview = {
        .uri = "/light_preview",
        .method = HTTP_POST,
        .handler = post_light_preview_req,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_post_light_preview);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    httpd_uri_t uri_post_light_config = {
        .uri = "/light_config",
        .method = HTTP_POST,
        .handler = post_light_config_req,
    };
    ret = httpd_register_uri_handler(sever_handle, &uri_post_light_config);
    if (ret != ESP_OK) {
        web_ws_stop();
        return ret;
    }

    return ESP_OK;
}

esp_err_t web_ws_stop(void) {
    if (sever_handle) {
        httpd_stop(sever_handle);
        sever_handle = NULL;
    }
    http_html = NULL;
    ws_receive_fn = NULL;
    client_fds = -1;
    return ESP_OK;
}

esp_err_t web_ws_send(uint8_t *data, int len) {
    if (sever_handle == NULL || client_fds < 0 || data == NULL || len <= 0) {
        return ESP_FAIL;
    }
    httpd_ws_frame_t pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.payload = data;
    pkt.len = len;
    pkt.type = HTTPD_WS_TYPE_TEXT;
    return httpd_ws_send_data(sever_handle, client_fds, &pkt);
}
