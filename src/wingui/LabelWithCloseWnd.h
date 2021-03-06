/* Copyright 2019 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

struct LabelWithCloseWnd {
    LabelWithCloseWnd() = default;
    ~LabelWithCloseWnd() = default;

    bool Create(HWND parent, int cmd);
    void SetLabel(const WCHAR*);
    void SetFont(HFONT);
    void SetBgCol(COLORREF);
    void SetTextCol(COLORREF);
    void SetPaddingXY(int x, int y);
    SizeI GetIdealSize();

    HWND hwnd = 0;
    HFONT font = 0;
    int cmd = 0;

    RectI closeBtnPos = {};
    COLORREF txtCol = 0;
    COLORREF bgCol = 0;

    // in points
    int padX = 0;
    int padY = 0;
};
