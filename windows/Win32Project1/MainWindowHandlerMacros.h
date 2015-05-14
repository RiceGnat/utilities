// Convenience macros
#define WINDOW_REDRAW pWindow->Redraw

#define WINDOW_TITLE pWindow->GetTitle()

#define WINDOW_DATA pWindow->GetData<Data>()
#define WINDOW_MOUSEDOWN WINDOW_DATA.isMouseDown
#define WINDOW_MOUSETRACKING WINDOW_DATA.isMouseTracking

// Contains both x and y arguments
#define WINDOW_CAPTIONPOSITION layout.paddingX, layout.paddingY
// Contains both x and y arguments
#define WINDOW_LINEPOSITION(lineNum) WINDOW_CAPTIONPOSITION + layout.captionHeight + layout.lineHeight * lineNum

#define WINDOW_CAPTIONWIDTH 
