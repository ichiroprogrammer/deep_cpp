#ifdef SANITIZER  // sanitizerはnew/deleteオーバーロードのテストは誤動作する
#define NewDelete_Opt DISABLED_NewDelete
#else
#define NewDelete_Opt NewDelete
#endif
