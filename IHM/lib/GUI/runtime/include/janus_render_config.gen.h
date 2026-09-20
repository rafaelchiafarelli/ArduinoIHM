#ifndef JANUS_GEN_RENDER_CONFIG_H
#define JANUS_GEN_RENDER_CONFIG_H

/* app.yaml: display.render_mode: blocking — the fixed runtime's
 * polled/async render path is left out of the build. */
/* panel size — for janus_clear_screen's full-panel erase and/or the nav bar */
#define JANUS_DISPLAY_PANEL_W 320
#define JANUS_DISPLAY_PANEL_H 480

#endif  /* JANUS_GEN_RENDER_CONFIG_H */
