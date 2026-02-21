/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== index.html ==================
static const unsigned char temp_binary_data_0[] =
"<!DOCTYPE html>\n"
"<html lang=\"en\">\n"
"<head>\n"
"    <meta charset=\"UTF-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"    <title>Plugin UI</title>\n"
"    <style>\n"
"        /* Minimal reset \xe2\x80\x94 replace with your own styles */\n"
"        body { margin: 0; padding: 16px; font-family: sans-serif; background: #1a1a1a; color: #eee; }\n"
"        button { padding: 8px 16px; cursor: pointer; }\n"
"        #log { margin-top: 12px; font-size: 12px; color: #aaa; }\n"
"    </style>\n"
"</head>\n"
"<body>\n"
"\n"
"<h2>Plugin Template</h2>\n"
"\n"
"<!-- Test button: triggers a C++ callback to verify bidirectional comms are wired -->\n"
"<button id=\"pingBtn\">Send test message to C++</button>\n"
"<div id=\"log\">Messages from C++ will appear here.</div>\n"
"\n"
"<script>\n"
"// =============================================================================\n"
"// JS <-> C++ Bridge (JUCE 8 WebBrowserComponent native integration)\n"
"// =============================================================================\n"
"//\n"
"// SENDING to C++ (JS -> C++):\n"
"//   Call sendToPlugin(type, name, value) from anywhere in this page.\n"
"//   Uses window.__JUCE__.backend.emitEvent(\"pluginMessage\", { type, name, value })\n"
"//   which routes to the withEventListener(\"pluginMessage\", ...) callback in C++.\n"
"//\n"
"//   Message format \xe2\x80\x94 JSON object with three fields:\n"
"//   {\n"
"//     \"type\":  string  \xe2\x80\x94 message category, e.g. \"paramChange\", \"ping\"\n"
"//     \"name\":  string  \xe2\x80\x94 parameter ID or action name, e.g. \"gain\"\n"
"//     \"value\": number|string \xe2\x80\x94 the value, e.g. 0.75\n"
"//   }\n"
"//\n"
"//   Example: sendToPlugin(\"paramChange\", \"gain\", 0.75)\n"
"//\n"
"// RECEIVING from C++ (C++ -> JS):\n"
"//   C++ calls emitEventIfBrowserIsVisible(\"pluginMessage\", var) on the\n"
"//   WebBrowserComponent. Register a listener with:\n"
"//   window.__JUCE__.backend.addEventListener(\"pluginMessage\", handler)\n"
"//\n"
"// =============================================================================\n"
"\n"
"/**\n"
" * Send a message to the C++ plugin host.\n"
" * @param {string} type  - Message category (e.g. \"paramChange\", \"ping\")\n"
" * @param {string} name  - Parameter ID or action (e.g. \"gain\")\n"
" * @param {*}      value - Value to send (number, string, or boolean)\n"
" */\n"
"function sendToPlugin(type, name, value) {\n"
"    if (window.__JUCE__ && window.__JUCE__.backend) {\n"
"        // JUCE 8 WebBrowserComponent native bridge\n"
"        // emitEvent sends { eventId, payload } via window.__JUCE__.postMessage\n"
"        // C++ withEventListener(\"pluginMessage\", ...) receives the payload object\n"
"        window.__JUCE__.backend.emitEvent(\"pluginMessage\", { type: type, name: name, value: value });\n"
"    } else {\n"
"        // Fallback for browser-based development (no C++ host)\n"
"        console.log('[sendToPlugin]', JSON.stringify({ type: type, name: name, value: value }));\n"
"    }\n"
"}\n"
"\n"
"// Listen for messages sent from C++ via emitEventIfBrowserIsVisible(\"pluginMessage\", ...)\n"
"// C++ sends a var object; JUCE serialises it and delivers it here as a JS object.\n"
"function setupBridgeListeners() {\n"
"    if (window.__JUCE__ && window.__JUCE__.backend) {\n"
"        window.__JUCE__.backend.addEventListener(\"pluginMessage\", function (msg) {\n"
"            try {\n"
"                const log = document.getElementById('log');\n"
"                log.textContent = 'From C++: type=' + msg.type + ' name=' + msg.name + ' value=' + msg.value;\n"
"                console.log('[pluginMessageReceived]', msg);\n"
"            } catch (e) {\n"
"                console.error('[pluginMessageReceived] error:', e, msg);\n"
"            }\n"
"        });\n"
"    }\n"
"}\n"
"\n"
"// Set up listeners once the JUCE backend is available.\n"
"// The backend is injected before any page scripts run when native integration is enabled.\n"
"if (document.readyState === 'loading') {\n"
"    document.addEventListener('DOMContentLoaded', setupBridgeListeners);\n"
"} else {\n"
"    setupBridgeListeners();\n"
"}\n"
"\n"
"// Test button: sends a \"ping\" message to C++ to verify the bridge is working.\n"
"document.getElementById('pingBtn').addEventListener('click', function () {\n"
"    sendToPlugin('ping', 'test', 1);\n"
"});\n"
"\n"
"</script>\n"
"</body>\n"
"</html>\n";

const char* index_html = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x2c834af8:  numBytes = 3981; return index_html;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "index_html"
};

const char* originalFilenames[] =
{
    "index.html"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
