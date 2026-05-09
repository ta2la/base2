#include "CmdBridge.h"

#include "CmdSys.h"

#ifdef Q_OS_WASM

#include "CmdExeGuard.h"

#include <QString>
#include <emscripten.h>
#include <emscripten/em_js.h>

// Push a single line into window.craseLog and the REPL output (if mounted).
EM_JS(void, crase_log_push_, (const char* line), {
    const s = UTF8ToString(line);
    window.craseLog = window.craseLog || [];
    window.craseLog.push(s);
    const out = document.getElementById('craseOut');
    if (out) {
        out.textContent += s + '\n';
        out.scrollTop = out.scrollHeight;
    }
});

// Define a convenience wrapper window.craseRun(cmd) → returns log appended
// since the call. Idempotent — installed once at first init.
EM_JS(void, crase_install_run_helper_, (), {
    if (window.craseRun) return;
    window.craseRun = function(cmd) {
        const before = (window.craseLog || []).length;
        Module.ccall('craseExec', null, ['string'], [cmd]);
        return (window.craseLog || []).slice(before).join('\n');
    };
});

// DOM REPL panel: <input id=craseCmd> + <pre id=craseOut>. Driven by the
// generic Chrome Browser MCP plugin via type / press Enter / snapshot — no
// extra install needed beyond the plugin itself. Idempotent.
EM_JS(void, crase_repl_install_, (), {
    if (document.getElementById('craseCmd')) return;
    const wrap = document.createElement('div');
    wrap.style.cssText =
        'position:fixed;right:8px;top:8px;z-index:99999;'
      + 'background:#fff;border:1px solid #888;padding:6px;'
      + 'font-family:monospace;font-size:12px;'
      + 'box-shadow:0 2px 6px rgba(0,0,0,0.15)';
    const label = document.createElement('div');
    label.textContent = 'MCP DOM CONNECTOR';
    label.title = 'click to expand / collapse';
    label.style.cssText = 'font-weight:bold;color:#333;cursor:pointer;'
                       + 'user-select:none;-webkit-user-select:none';
    const body = document.createElement('div');
    body.style.cssText = 'display:none;margin-top:4px';
    const input = document.createElement('input');
    input.id = 'craseCmd';
    input.type = 'text';
    input.placeholder = 'enter command, hit Enter';
    input.style.cssText = 'width:480px;padding:3px;font-family:monospace';
    const out = document.createElement('pre');
    out.id = 'craseOut';
    out.style.cssText =
        'width:480px;height:280px;margin:4px 0 0;padding:4px;'
      + 'background:#f4f4f4;border:1px solid #ddd;overflow:auto;'
      + 'white-space:pre-wrap;font-family:monospace;font-size:11px';
    body.appendChild(input);
    body.appendChild(out);
    wrap.appendChild(label);
    wrap.appendChild(body);
    document.body.appendChild(wrap);

    label.addEventListener('click', function(){
        const open = body.style.display !== 'none';
        body.style.display = open ? 'none' : 'block';
        if (!open) input.focus();
    });

    input.addEventListener('keydown', function(e){
        if (e.key !== 'Enter') return;
        const cmd = input.value;
        if (!cmd) return;
        input.value = '';
        out.textContent += '> ' + cmd + '\n';
        // crase_log_push_ updates #craseOut live, incl. QTimer-deferred lines.
        Module.ccall('craseExec', null, ['string'], [cmd]);
    });
});

extern "C" EMSCRIPTEN_KEEPALIVE
void craseExec(const char* cmd) {
    if (!cmd) return;
    CMD_SYS.execute_threadSafe(QString::fromUtf8(cmd), "wasm", -1, "EXTERNAL_JS");
}

class WasmCmdGuard : public CmdExeGuard {
public:
    WasmCmdGuard() : CmdExeGuard(Update) { CMD_SYS.reg(this); }
    void commandExecuted() override {
        QString line = CMD_SYS.executingArgs_.toString();
        crase_log_push_(line.toUtf8().constData());
    }
};

void CmdBridge::init() {
    static WasmCmdGuard guard;       // registers itself with CMD_SYS
    crase_install_run_helper_();     // window.craseRun wrapper for DevTools
    crase_repl_install_();           // DOM REPL panel for generic Browser MCP
}

#else // native

#include "WsServerLiteGuard.h"

void CmdBridge::init() {
    WsServerLiteGuard::inst();       // existing WS server on ws://127.0.0.1:12345
}

#endif
