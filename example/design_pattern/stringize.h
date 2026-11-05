// @@@ sample begin 0:0

// 二段階文字列化イデオムマクロ
#define STRINGIZE_INTERNAL(x) #x
#define STRINGIZE(x) STRINGIZE_INTERNAL(x)
// @@@ sample end
// @@@ sample begin 0:1

#define WHERE __FILE__ ":" STRINGIZE(__LINE__)
// @@@ sample end
