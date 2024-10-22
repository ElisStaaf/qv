/*** includes ***/

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/*** defines ***/

#define QV_VERSION "1.1.1"
#define QV_TAB_STOP 8
#define QV_QUIT_TIMES 3

#define CTRL_KEY(k) ((k) & 0x1f)

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

enum editorHighlight {
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH
};

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

/*** data ***/

/* Need this to show syntax
 * highlighting properly. */
struct editorSyntax {
  char *filetype;
  char **filematch;
  char **keywords;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
};

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char *render;
  unsigned char *hl;
  int hl_open_comment;
} erow;

struct editorConfig {
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int screenrows;
  int screencols;
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct editorSyntax *syntax;
  struct termios orig_termios;
  int tab_stop;      // Changed from #define to a variable
  int quit_times;    // Changed from #define to a variable
};

struct editorConfig E;

/*** filetypes ***/

/* The "full" syntax highlighting list */
char *C_HL_extensions[] = { ".c", ".h", ".cpp", NULL };
char *C_HL_keywords[] = {
    "switch", "if", "while", "for", "break", "continue", "return", "else",
    "struct", "union", "typedef", "static", "enum", "case", "#define", 
    "#include", "#if", "#else", "#ifdef", "#ifndef", "#error", "#warning",
    "#endif", "goto",

    "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
    "void|", "true|", "false|", "NULL|", "auto|", "const|"
};

char *SH_HL_extensions[] = { ".sh", ".bashrc", ".profile", ".bash_profile", "Makefile", NULL };
char *SH_HL_keywords[] = {
    "case", "esac", "if", "fi", "elif", "else", "then", "echo", "alias",
    "expr", "for", "in", "function", "return", "done", "grep", "find",

    "local|", "export|", "set|", "true|", "false|"
};

char *GO_HL_extensions[] = { ".go", NULL };
char *GO_HL_keywords[] = {
    "switch", "case", "if", "else", "go", "package", "import",
    "return", "func", "for", "default", "goto", "fallthrough", "interface",

    "int|", "string|", "var|", "const|", "map|", "defer", "break|", "continue|",
    "range|", "type|", "uint|", "rune|", "byte|", "true|", "false|", "nil|", 
    "any|", "float32|", "int32|", "print|", "append|", "min|", "max|", "make|",
    "clear|", "panic|", "println|", "complex|", "new|"
};

char *PY_HL_extensions[] = { ".py", "pyi", NULL };
char *PY_HL_keywords[] = {
    "match", "case", "if", "else", "elif", "import", "print", "range",
    "in", "for", "while", "map", "zip", "from", "class", "def", "lambda",
    "return", "with", "is", "not", "try", "except", "finally", "yield", "or",
    "pass", "nonlocal", "global", "as", "assert", "yield", "and",

    "int|", "str|", "float|", "any|", "None|", "True|", "False|", "continue|",
    "break|", "raise|", "exec|", "eval|", "len|", "super|", "__import__|"
};

char *JS_HL_extensions[] = { ".js", ".ts", NULL };
char *JS_HL_keywords[] = {
    "function", "if", "else", "eval", "goto", "try", "catch", "finally",
    "import", "for", "int", "number", "string", "this", "void", "yield",
    "while", "char", "break", "continue", "do", "in", "typeof", "boolean",

    "let|", "const|", "private|", "public|", "true|", "false|", "interface|",
    "class|", "export|", "extends|", "switch|", "case|", "new|"
};

char *CS_HL_extensions[] = { ".cs", NULL };
char *CS_HL_keywords[] = {
    "namespace", "class", "if", "do", "else", "int", "string", "void", "return",
    "is", "in", "interface", "stackalloc", "double", "while", "char", "float"

    "const|", "let|", "var|", "true|", "false|", "null|", "new|", "out|", 
    "internal|", "enum|", "as|", "using|", "default|", "event|"
};

char *HTML_HL_extensions[] = { ".htm", ".html", NULL };
char *HTML_HL_keywords[] = {
    "<html>", "</html>", "<head>", "</head>", "<body>", "</body>", "<link>", 
    "<meta>", "<title>", "</title>", "<script>", "</script>",

    "<div>|", "</div>|", "<p>|", "</p>|", "<h1>|", "</h1>|", "<svg>|", "</svg>|",
    "<button>|", "</button>|", "<a>|", "</a>|", "<li>|", "</li>|", "<style>|",
    "</style>|"
};

char *RUST_HL_extensions[] = { ".rs", ".rlib", NULL };
char *RUST_HL_keywords[] = {
    "fn", "for", "if", "where", "while", "struct", "static", "str", "char", "!",
    "u8", "u16", "u32", "u64", "u128", "i8", "i16", "i32", "i64", "i128", "loop",
    "crate", "continue", "break", "as", "ref", "Self", "in", "do", "final", "priv",

    "let|", "mut|", "pub|", "const|", "return|", "self|", "async|", "await|", "true|",
    "false|", "type|", "typeof|", "yield|", "try|", "union|", "dyn|"
};

char *CSS_HL_extensions[] = { ".css", NULL };
char *CSS_HL_keywords[] = {
    "color", "backround-color", "transition-duration", "calc", "all", "content", "padding",
    "quotes", "border", "z-index",

    "@charset|", "@media|", "@keyframes|", "@import|", "@font-feature-values|", 
    "@font-face|"
};

char *RUBY_HL_extensions[] = { ".rb" ".erb", NULL };
char *RUBY_HL_keywords[] = {
    "alias", "and", "begin", "end", "def", "do", "case", "class", "if", "elsif", "else",
    "redo", "retry", "return", "self", "super", "when", "while", "yield", "or", "in"

    "true|", "false|", "unless|", "undef|", "until|", "nil|", "module|", "defined?|",
    "BEGIN|", "END|", "__FILE__|", "__LINE__|", "__ENCODING__|"
};

char *D_HL_extensions[] = { ".d", NULL };
char *D_HL_keywords[] = {
    "module", "extern", "public", "private", "import", "char", "int", "enum", "alias",
    "ubyte", "string", "static", "void", "long", "while", "if", "switch", "case" "return",

    "true|", "false|", "null|" 
};

char *SQL_HL_extensions[] = { ".sql", NULL };
char *SQL_HL_keywords[] = {
    "ADD", "ADD CONSTRAINT", "ALL", "ALTER", "COLUMN", "JOIN", "TABLE", "SELECT", "UNION",
    "PROCEDURE", "OR", "ORDER BY", "ROWNUM", "IS", "IN", "NULL", "EXEC", "DROP", "FROM"
};

char *LUA_HL_extensions[] = { ".lua", NULL };
char *LUA_HL_keywords[] = {
    "and", "or", "if", "else", "do", "while", "function", "end", "not", "return", "until",
    "repeat", "in", "for", "then", "break", "elseif",

    "local|", "true|", "false|", "nil|"
};

char *PWSH_HL_extensions[] = { ".ps1", NULL };
char *PWSH_HL_keywords[] = {
    "Write-Host", "Write-Output", "Clear-Host", "function", "if", "else", "try", "catch",
    "throw", "elseif", "echo", "cls", "dir", "ls", "cd",

    "$true|", "$false|", "$null|"
};

char *BAT_HL_extensions[] = { ".bat", NULL };
char *BAT_HL_keywords[] = {
    "@ECHO", "ECHO", "OFF", "SET", "PING", "CD", "CURL", "IF", "ELSE", "FIND" "COPY", "CURL", 
    "ON", "EXIT", "LS", "CLS", "PAUSE", "TITLE", "REM", "IPCONFIG", "TRACERT",

    ">>|", "+|", "-|", "*|", "/|"
};

char *GD_HL_extensions[] = { ".gd", NULL };
char *GD_HL_keywords[] = {
    "func", "var", "const", "class_name", "extends", "if", "elif", "else", "match", "return",
    "super", "print", "class", "class", "pass", "static", "enum", "breakpoint", "self", "is",
    "in", "as", "signal", "preload", "await", "yield", "void", "or",

    "@export|", "@icon|", "true|", "false|", "null|"
};

char *JAVA_HL_extensions[] = { ".java", NULL };
char *JAVA_HL_keywords[] = {
    "abstract", "continue", "for", "new", "switch" "assert", "default", "goto" "package", 
    "synchronized", "boolean", "do", "if", "private", "this", "break", "double", "implements",	
    "protected", "throw", "byte", "else", "import", "public", "throws", "case", "enum",
    "return", "while",

    "true|", "false|"
};

char *PHP_HL_extensions[] = { ".php", NULL };
char *PHP_HL_keywords[] = {
    "__halt_compiler|", "abstract", "and", "array", "as", "break", "callable", "case", "catch", "class", "clone", "const", 
    "continue", "declare", "default", "die", "do", "echo", "else", "elseif", "empty", "enddeclare", "endfor", "endforeach", 
    "endif", "endswitch", "endwhile", "eval", "exit", "extends", "final", "for", "foreach", "function", "global", "goto", "if",
    "implements", "include", "include_once", "instanceof", "insteadof", "interface", "isset", "list", "namespace", "new", "or",
    "print", "private", "protected", "public", "require", "require_once", "return", "static", "switch", "throw", "trait", "try",
    "unset", "use", "var", "while", "xor" 
    /* Thank you Chris from https://www.php.net/manual/en/reserved.keywords.php, you made my life so much easier! */
};

char *VIM_HL_extensions[] = { ".vim", ".vimrc", NULL };
char *VIM_HL_keywords[] = {
    "function!", "let", "if", "else", "return", "endif", "endfunction", "echo", "len", "strlen",
    "strchars", "split", "join", "tolower", "toupper", "abort", "echon", "for", "in", "continue",
    "break", "endfor", "while", "endwhile", "command!", "empty", "sort", "copy",

    "=|", "=~|", "!=|", "!~|", "&&|", "|||"
};

char *KOTL_HL_extensions[] = { ".kt", ".kts", ".ktm", NULL };
char *KOTL_HL_keywords[] = {
    "as", "break", "class", "continue", "do", "else", "for", "fun", "if", "in", "interface", "is",
    "object", "package", "super", "throw", "try", "typealias", "typeof", "val", "var", "when",
    "while", "public", "override", "private", "field",

    "false|", "true|", "null|", "return|", "this|"
};

char *SWFT_HL_extensions[] = { ".swift", NULL };
char *SWFT_HL_keywords[] = {
    "import", "func", "struct", "var", "let", "class", "enum", "open", "extension", "init", "public",
    "static", "typealias", "switch", "case", "break", "continue", "if", "else", "where", "while", "for",
    "in", "repeat", "throw", "catch", "try",

    "return|", "true|", "false|", "default|", "nil|", "Any|"
};

char *DART_HL_extensions[] = { ".dart", NULL };
char *DART_HL_keywords[] = {
    "switch", "assert", "finally", "case", "if", "else", "break", "continue", "in", "is", "final", "try",
    "catch", "throw", "rethrow", "super", "extends", "as", "export", "import",

    "true|", "false|", "null|", "Function|", "const|", "class|", "new|", "var|", "this|", "async|", "sync|",
    "interface|"
};

char *ZIG_HL_extensions[] = { ".zig", NULL };
char *ZIG_HL_keywords[] = {
    "const", "try", "return", "and", "or", "var", "test", "extern", "comptime", "for", "break", "if", "deer",
    "while", "else", "switch",

    "@import|", "void|", "i64|", "u32|", "pub|", "fn|", "true|", "false|", "@TypeOf|", "null|", "undefined|",
    "@compileError|", "@as", "@setFloatMode|", "@intCast|", "@Vector|", "i32|", "u64|", "i8|", "u8|", "anyerror|",
    "c_int|", "usize|", "f32|", "f64|"
};

char *ASM_HL_extensions[] = { ".s", ".asm", ".asem", NULL };
char *ASM_HL_keywords[] = {
    "pushl", "movl", "call", "subl", "leave", "ret", "movq", "subq", "cmpl", "jmp", "je", "jg", "jge", "jl",
    "jle", "jne", "add", "and", "div", "in", "dec", "cwd", "inc", "int", "into", "iret", "lea", "les", "lahf",
    "lodsb", "loc", "mov", "movsb", "movsw", "neg", "mul", "not", "or", "sub", "rol", "ror", "xor", "wait",

    ".section|", ".globl|", ".def|", ".file|", ".text|", ".ascii|", ".data|"
};

char *CBL_HL_extensions[] = { ".cbl", ".cobol", ".cob", NULL };
char *CBL_HL_keywords[] = {
    "IDENTIFICATION", "DIVISION", "PROGRAM-ID", "DATA", "FILE", "SECTION", "WORKING-STORAGE", "PIC", "VALUE",
    "PROCEDURE", "MESSAGE", "DISPLAY", "PROGRAM", "MAIN-PROCEDURE", "STOP", "RUN", "END"
};

char *EX_HL_extensions[] = { ".ex", ".exs", NULL };
char *EX_HL_keywords[] = {
    "import", "except", "if", "unless", "do", "use", "when", "and", "raise", "case", "alias", "else",
    "with", "exit", "@spec", "only",

    "Kernel|", "def|", "defmodule|", "end|", "defp|", "nil|", "true|", "false|", "defstruct|", "IO|"
};

char *HS_HL_extensions[] = { ".hsc", ".hs", NULL};
char *HS_HL_keywords[] = {
    "as", "case", "of", "family", "instance", "default", "deriving", "forall", "foreign", "hiding"
    "infix", "infixl", "infixr", "instance", "in", "newtype", "qualified", "rec", "type", "where",

    "class|", "data|", "do|", "if|", "then|", "else|", "import|", "let|", "mdo|", "module|", "proc"
};

char *R_HL_extensions[] = { ".r", ".rd", ".rsx", NULL};
char *R_HL_keywords[] = {
    "if", "else", "while", "repeat", "for", "in", "next", "break", "print", "paste", "return", "c",
    "help", "library", "install.packages",

    "function|", "TRUE|", "FALSE|", "NULL|", "Inf|", "NaN|", "NA|",
};

/* HLDB stands for HighLighting DataBase, and contains 
 * the settings and initialization for the syntax highlighting */
struct editorSyntax HLDB[] = {
  {
    "C/C++",
    C_HL_extensions,
    C_HL_keywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
   "Shell",
    SH_HL_extensions,
    SH_HL_keywords,
    "#", ":'", "'",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Golang",
    GO_HL_extensions,
    GO_HL_keywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,  
  },
  {
    "Python",
    PY_HL_extensions,
    PY_HL_keywords,
    "#", "\"\"\"", "\"\"\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "JS/TS",
    JS_HL_extensions,
    JS_HL_keywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "C#",
    CS_HL_extensions,
    CS_HL_keywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "HTML",
    HTML_HL_extensions,
    HTML_HL_keywords,
    "<!DOCTYPE html>", "<!--", "-->",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Rust",
    RUST_HL_extensions,
    RUST_HL_keywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "CSS",
    CSS_HL_extensions,
    CSS_HL_keywords,
    "/**", "/*", "/*",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Ruby",
    RUBY_HL_extensions,
    RUBY_HL_keywords,
    "#", "=begin", "=end",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "D",
    D_HL_extensions,
    D_HL_keywords, 
    "//", "/+", "+/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "SQL",
    SQL_HL_extensions,
    SQL_HL_keywords, 
    "--", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Lua",
    LUA_HL_extensions,
    LUA_HL_keywords, 
    "--", "--[[", "]]",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Powershell",
    PWSH_HL_extensions,
    PWSH_HL_keywords, 
    "#", "<#", "#>",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Batch",
    BAT_HL_extensions,
    BAT_HL_keywords, 
    "::", "goto comment", ":comment",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "GDScript",
    GD_HL_extensions,
    GD_HL_keywords, 
    "#", "\"\"\"", "\"\"\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Java",
    JAVA_HL_extensions,
    JAVA_HL_keywords, 
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "PHP",
    PHP_HL_extensions,
    PHP_HL_keywords, 
    "#", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Vim Script",
    PHP_HL_extensions,
    PHP_HL_keywords, 
    "\"", "\"\"\"", "\"\"\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Kotlin",
    KOTL_HL_extensions,
    KOTL_HL_keywords, 
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Swift",
    SWFT_HL_extensions,
    SWFT_HL_keywords, 
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Dart",
    DART_HL_extensions,
    DART_HL_keywords, 
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Zig",
    ZIG_HL_extensions,
    ZIG_HL_keywords, 
    "//", "\\\\", "\\\\",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Assembly",
    ASM_HL_extensions,
    ASM_HL_keywords, 
    ";", "COMMENT @", "@",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "COBOL",
    CBL_HL_extensions,
    CBL_HL_keywords, 
    "*>", "**********", "***************",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Elixir",
    EX_HL_extensions,
    EX_HL_keywords, 
    "#", "@doc \"\"\"", "\"\"\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "Haskell",
    HS_HL_extensions,
    HS_HL_keywords, 
    "--", "{-", "-}",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
  {
    "R",
    HS_HL_extensions,
    HS_HL_keywords, 
    "#", "\"", "\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
  },
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

/*** prototypes ***/

void editorSetStatusMessage(const char *fmt, ...);
void editorRefreshScreen();
char *editorPrompt(char *prompt, void (*callback)(char *, int));

/*** terminal ***/

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }

  if (c == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }

    return '\x1b';
  } else {
    return c;
  }
}

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;

  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';

  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

  return 0;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

/*** syntax highlighting ***/

int is_separator(int c) {
  return isspace(c) || c == '\0' || strchr(",.()+-/*=@#~&%<>[]{}!\\:|;", c) != NULL;
}

void editorUpdateSyntax(erow *row) {
  row->hl = realloc(row->hl, row->rsize);
  memset(row->hl, HL_NORMAL, row->rsize);

  if (E.syntax == NULL) return;

  char **keywords = E.syntax->keywords;

  char *scs = E.syntax->singleline_comment_start;
  char *mcs = E.syntax->multiline_comment_start;
  char *mce = E.syntax->multiline_comment_end;

  int scs_len = scs ? strlen(scs) : 0;
  int mcs_len = mcs ? strlen(mcs) : 0;
  int mce_len = mce ? strlen(mce) : 0;

  int prev_sep = 1;
  int in_string = 0;
  int in_comment = (row->idx > 0 && E.row[row->idx - 1].hl_open_comment);

  int i = 0;
  while (i < row->rsize) {
    char c = row->render[i];
    unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

    if (scs_len && !in_string && !in_comment) {
      if (!strncmp(&row->render[i], scs, scs_len)) {
        memset(&row->hl[i], HL_COMMENT, row->rsize - i);
        break;
      }
    }

    if (mcs_len && mce_len && !in_string) {
      if (in_comment) {
        row->hl[i] = HL_MLCOMMENT;
        if (!strncmp(&row->render[i], mce, mce_len)) {
          memset(&row->hl[i], HL_MLCOMMENT, mce_len);
          i += mce_len;
          in_comment = 0;
          prev_sep = 1;
          continue;
        } else {
          i++;
          continue;
        }
      } else if (!strncmp(&row->render[i], mcs, mcs_len)) {
        memset(&row->hl[i], HL_MLCOMMENT, mcs_len);
        i += mcs_len;
        in_comment = 1;
        continue;
      }
    }

    if (E.syntax->flags & HL_HIGHLIGHT_STRINGS) {
      if (in_string) {
        row->hl[i] = HL_STRING;
        if (c == '\\' && i + 1 < row->rsize) {
          row->hl[i + 1] = HL_STRING;
          i += 2;
          continue;
        }
        if (c == in_string) in_string = 0;
        i++;
        prev_sep = 1;
        continue;
      } else {
        if (c == '"' || c == '\'') {
          in_string = c;
          row->hl[i] = HL_STRING;
          i++;
          continue;
        }
      }
    }

    if (E.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
      if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) ||
          (c == '.' && prev_hl == HL_NUMBER)) {
        row->hl[i] = HL_NUMBER;
        i++;
        prev_sep = 0;
        continue;
      }
    }

    if (prev_sep) {
      int j;
      for (j = 0; keywords[j]; j++) {
        int klen = strlen(keywords[j]);
        int kw2 = keywords[j][klen - 1] == '|';
        if (kw2) klen--;

        if (!strncmp(&row->render[i], keywords[j], klen) &&
            is_separator(row->render[i + klen])) {
          memset(&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
          i += klen;
          break;
        }
      }
      if (keywords[j] != NULL) {
        prev_sep = 0;
        continue;
      }
    }

    prev_sep = is_separator(c);
    i++;
  }

  int changed = (row->hl_open_comment != in_comment);
  row->hl_open_comment = in_comment;
  if (changed && row->idx + 1 < E.numrows)
    editorUpdateSyntax(&E.row[row->idx + 1]);
}

int editorSyntaxToColor(int hl) {
  switch (hl) {
    case HL_COMMENT:
    case HL_MLCOMMENT: return 36;
    case HL_KEYWORD1: return 33;
    case HL_KEYWORD2: return 32;
    case HL_STRING: return 35;
    case HL_NUMBER: return 31;
    case HL_MATCH: return 34;
    default: return 37;
  }
}

void editorSelectSyntaxHighlight() {
  E.syntax = NULL;
  if (E.filename == NULL) return;

  char *ext = strrchr(E.filename, '.');

  for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
    struct editorSyntax *s = &HLDB[j];
    unsigned int i = 0;
    while (s->filematch[i]) {
      int is_ext = (s->filematch[i][0] == '.');
      if ((is_ext && ext && !strcmp(ext, s->filematch[i])) ||
          (!is_ext && strstr(E.filename, s->filematch[i]))) {
        E.syntax = s;

        int filerow;
        for (filerow = 0; filerow < E.numrows; filerow++) {
          editorUpdateSyntax(&E.row[filerow]);
        }

        return;
      }
      i++;
    }
  }
}

/*** row operations ***/

int editorRowCxToRx(erow *row, int cx) {
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (QV_TAB_STOP - 1) - (rx % QV_TAB_STOP);
    rx++;
  }
  return rx;
}

int editorRowRxToCx(erow *row, int rx) {
  int cur_rx = 0;
  int cx;
  for (cx = 0; cx < row->size; cx++) {
    if (row->chars[cx] == '\t')
      cur_rx += (QV_TAB_STOP - 1) - (cur_rx % QV_TAB_STOP);
    cur_rx++;

    if (cur_rx > rx) return cx;
  }
  return cx;
}

void editorUpdateRow(erow *row) {
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t') tabs++;

  free(row->render);
  row->render = malloc(row->size + tabs*(QV_TAB_STOP - 1) + 1);

  int idx = 0;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';
      while (idx % QV_TAB_STOP != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;

  editorUpdateSyntax(row);
}

void editorInsertRow(int at, char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;

  E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
  memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));
  for (int j = at + 1; j <= E.numrows; j++) E.row[j].idx++;

  E.row[at].idx = at;

  E.row[at].size = len;
  E.row[at].chars = malloc(len + 1);
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';

  E.row[at].rsize = 0;
  E.row[at].render = NULL;
  E.row[at].hl = NULL;
  E.row[at].hl_open_comment = 0;
  editorUpdateRow(&E.row[at]);

  E.numrows++;
  E.dirty++;
}

void editorFreeRow(erow *row) {
  free(row->render);
  free(row->chars);
  free(row->hl);
}

void editorDelRow(int at) {
  if (at < 0 || at >= E.numrows) return;
  editorFreeRow(&E.row[at]);
  memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
  for (int j = at; j < E.numrows - 1; j++) E.row[j].idx--;
  E.numrows--;
  E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c) {
  if (at < 0 || at > row->size) at = row->size;
  row->chars = realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  editorUpdateRow(row);
  E.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len) {
  row->chars = realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  E.dirty++;
}

void editorRowDelChar(erow *row, int at) {
  if (at < 0 || at >= row->size) return;
  memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
  row->size--;
  editorUpdateRow(row);
  E.dirty++;
}

/*** editor operations ***/

void editorInsertChar(int c) {
  if (E.cy == E.numrows) {
    editorInsertRow(E.numrows, "", 0);
  }
  editorRowInsertChar(&E.row[E.cy], E.cx, c);
  E.cx++;
}

void editorInsertNewline() {
  if (E.cx == 0) {
    editorInsertRow(E.cy, "", 0);
  } else {
    erow *row = &E.row[E.cy];
    editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
    row = &E.row[E.cy];
    row->size = E.cx;
    row->chars[row->size] = '\0';
    editorUpdateRow(row);
  }
  E.cy++;
  E.cx = 0;
}

void editorDelChar() {
  if (E.cy == E.numrows) return;
  if (E.cx == 0 && E.cy == 0) return;

  erow *row = &E.row[E.cy];
  if (E.cx > 0) {
    editorRowDelChar(row, E.cx - 1);
    E.cx--;
  } else {
    E.cx = E.row[E.cy - 1].size;
    editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
    editorDelRow(E.cy);
    E.cy--;
  }
}

/*** file i/o ***/

char *editorRowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < E.numrows; j++)
    totlen += E.row[j].size + 1;
  *buflen = totlen;

  char *buf = malloc(totlen);
  char *p = buf;
  for (j = 0; j < E.numrows; j++) {
    memcpy(p, E.row[j].chars, E.row[j].size);
    p += E.row[j].size;
    *p = '\n';
    p++;
  }

  return buf;
}

void editorOpen(char *filename) {
  free(E.filename);
  E.filename = strdup(filename);

  editorSelectSyntaxHighlight();

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("[ERROR]: The requested file could not be opened.");
    return;
  }

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    editorInsertRow(E.numrows, line, linelen);
  }
  free(line);
  fclose(fp);
  E.dirty = 0;
}

void editorSave() {
  if (E.filename == NULL) {
    E.filename = editorPrompt("Save as: %s (ESC to cancel)", NULL);
    if (E.filename == NULL) {
      editorSetStatusMessage("Save aborted");
      return;
    }
    editorSelectSyntaxHighlight();
  }

  int len;
  char *buf = editorRowsToString(&len);

  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        E.dirty = 0;
        editorSetStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage("[ERROR]: Can't save! I/O error: %s", strerror(errno));
}

/*** find ***/

void editorFindCallback(char *query, int key) {
  static int last_match = -1;
  static int direction = 1;

  static int saved_hl_line;
  static char *saved_hl = NULL;

  if (saved_hl) {
    memcpy(E.row[saved_hl_line].hl, saved_hl, E.row[saved_hl_line].rsize);
    free(saved_hl);
    saved_hl = NULL;
  }

  if (key == '\r' || key == '\x1b') {
    last_match = -1;
    direction = 1;
    return;
  } else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
    direction = 1;
  } else if (key == ARROW_LEFT || key == ARROW_UP) {
    direction = -1;
  } else {
    last_match = -1;
    direction = 1;
  }

  if (last_match == -1) direction = 1;
  int current = last_match;
  int i;
  for (i = 0; i < E.numrows; i++) {
    current += direction;
    if (current == -1) current = E.numrows - 1;
    else if (current == E.numrows) current = 0;

    erow *row = &E.row[current];
    char *match = strstr(row->render, query);
    if (match) {
      last_match = current;
      E.cy = current;
      E.cx = editorRowRxToCx(row, match - row->render);
      E.rowoff = E.numrows;

      saved_hl_line = current;
      saved_hl = malloc(row->rsize);
      memcpy(saved_hl, row->hl, row->rsize);
      memset(&row->hl[match - row->render], HL_MATCH, strlen(query));
      break;
    }
  }
}

void editorFind() {
  int saved_cx = E.cx;
  int saved_cy = E.cy;
  int saved_coloff = E.coloff;
  int saved_rowoff = E.rowoff;

  char *query = editorPrompt("Search: %s (Use ESC/Arrows/Enter)",
                             editorFindCallback);

  if (query) {
    free(query);
  } else {
    E.cx = saved_cx;
    E.cy = saved_cy;
    E.coloff = saved_coloff;
    E.rowoff = saved_rowoff;
  }
}

/*** append buffer ***/

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL) return;
  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}

void abFree(struct abuf *ab) {
  free(ab->b);
}

/*** output ***/

void editorScroll() {
  E.rx = 0;
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }

  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenrows) {
    E.rowoff = E.cy - E.screenrows + 1;
  }
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  if (E.rx >= E.coloff + E.screencols) {
    E.coloff = E.rx - E.screencols + 1;
  }
}

void editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenrows; y++) {
    int filerow = y + E.rowoff;
    if (filerow >= E.numrows) {
      if (E.numrows == 0 && y == E.screenrows / 3) {
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
          "QV editor -- version %s", QV_VERSION);
        if (welcomelen > E.screencols) welcomelen = E.screencols;
        int padding = (E.screencols - welcomelen) / 2;
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);
        abAppend(ab, welcome, welcomelen);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0) len = 0;
      if (len > E.screencols) len = E.screencols;
      char *c = &E.row[filerow].render[E.coloff];
      unsigned char *hl = &E.row[filerow].hl[E.coloff];
      int current_color = -1;
      int j;
      for (j = 0; j < len; j++) {
        if (iscntrl(c[j])) {
          char sym = (c[j] <= 26) ? '@' + c[j] : '?';
          abAppend(ab, "\x1b[7m", 4);
          abAppend(ab, &sym, 1);
          abAppend(ab, "\x1b[m", 3);
          if (current_color != -1) {
            char buf[16];
            int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", current_color);
            abAppend(ab, buf, clen);
          }
        } else if (hl[j] == HL_NORMAL) {
          if (current_color != -1) {
            abAppend(ab, "\x1b[39m", 5);
            current_color = -1;
          }
          abAppend(ab, &c[j], 1);
        } else {
          int color = editorSyntaxToColor(hl[j]);
          if (color != current_color) {
            current_color = color;
            char buf[16];
            int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", color);
            abAppend(ab, buf, clen);
          }
          abAppend(ab, &c[j], 1);
        }
      }
      abAppend(ab, "\x1b[39m", 5);
    }

    abAppend(ab, "\x1b[K", 3);
    abAppend(ab, "\r\n", 2);
  }
}

void editorDrawStatusBar(struct abuf *ab) {
  abAppend(ab, "\x1b[7m", 4);
  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
    E.filename ? E.filename : "[No Name]", E.numrows,
    E.dirty ? "[+]" : "");
  int rlen = snprintf(rstatus, sizeof(rstatus), "%s | %d/%d",
    E.syntax ? E.syntax->filetype : "[No FT]", E.cy + 1, E.numrows);
  if (len > E.screencols) len = E.screencols;
  abAppend(ab, status, len);
  while (len < E.screencols) {
    if (E.screencols - len == rlen) {
      abAppend(ab, rstatus, rlen);
      break;
    } else {
      abAppend(ab, " ", 1);
      len++;
    }
  }
  abAppend(ab, "\x1b[m", 3);
  abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct abuf *ab) {
  abAppend(ab, "\x1b[K", 3);
  int msglen = strlen(E.statusmsg);
  if (msglen > E.screencols) msglen = E.screencols;
  if (msglen && time(NULL) - E.statusmsg_time < 5)
    abAppend(ab, E.statusmsg, msglen);
}

void editorRefreshScreen() {
  editorScroll();

  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab);
  editorDrawStatusBar(&ab);
  editorDrawMessageBar(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1,
                                            (E.rx - E.coloff) + 1);
  abAppend(&ab, buf, strlen(buf));

  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  E.statusmsg_time = time(NULL);
}

/*** input ***/

char *editorPrompt(char *prompt, void (*callback)(char *, int)) {
  size_t bufsize = 128;
  char *buf = malloc(bufsize);

  size_t buflen = 0;
  buf[0] = '\0';

  while (1) {
    editorSetStatusMessage(prompt, buf);
    editorRefreshScreen();

    int c = editorReadKey();
    if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (buflen != 0) buf[--buflen] = '\0';
    } else if (c == '\x1b') {
      editorSetStatusMessage("");
      if (callback) callback(buf, c);
      free(buf);
      return NULL;
    } else if (c == '\r') {
      if (buflen != 0) {
        editorSetStatusMessage("");
        if (callback) callback(buf, c);
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }

    if (callback) callback(buf, c);
  }
}

void editorMoveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      } else if (E.cy > 0) {
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
    case ARROW_RIGHT:
      if (row && E.cx < row->size) {
        E.cx++;
      } else if (row && E.cx == row->size) {
        E.cy++;
        E.cx = 0;
      }
      break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      if (E.cy < E.numrows) {
        E.cy++;
      }
      break;
  }

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}

void editorProcessKeypress() {
  static int quit_times = QV_QUIT_TIMES;

  int c = editorReadKey();

  switch (c) {
    case '\r':
      editorInsertNewline();
      break;

    case CTRL_KEY('q'):
      if (E.dirty && quit_times > 0) {
        editorSetStatusMessage("[WARNING]: File has unsaved changes. "
          "Press Ctrl-Q %d more times to quit.", quit_times);
        quit_times--;
        return;
      }
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;

    case CTRL_KEY('s'):
      editorSave();
      break;

    case HOME_KEY:
      E.cx = 0;
      break;

    case END_KEY:
      if (E.cy < E.numrows)
        E.cx = E.row[E.cy].size;
      break;

    case CTRL_KEY('f'):
      editorFind();
      break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
      if (c == DEL_KEY) editorMoveCursor(ARROW_RIGHT);
      editorDelChar();
      break;

    case PAGE_UP:
    case PAGE_DOWN:
      {
        if (c == PAGE_UP) {
          E.cy = E.rowoff;
        } else if (c == PAGE_DOWN) {
          E.cy = E.rowoff + E.screenrows - 1;
          if (E.cy > E.numrows) E.cy = E.numrows;
        }

        int times = E.screenrows;
        while (times--)
          editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;

    case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      editorInsertChar(c);
      break;
  }

  quit_times = QV_QUIT_TIMES;
}

/*** init ***/

void loadConfig(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("[ERROR]: Configuration file could not be opened");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *equals = strchr(line, '=');
        if (equals) {
            *equals = '\0'; // Null-terminate at '=' to split key
            char *key = line;
            char *value = equals + 1;

            // Remove any whitespace
            key[strcspn(key, "\r\n")] = 0; // remove trailing newline
            value[strcspn(value, "\r\n")] = 0; // remove trailing newline

            if (strcmp(key, "tab_stop") == 0) {
                E.tab_stop = atoi(value);
            } else if (strcmp(key, "quit_times") == 0) {
                E.quit_times = atoi(value);
            }
        }
    }

    fclose(file);
}

// Initialize the editor configuration
void initEditor() {
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.rowoff = 0;
    E.coloff = 0;
    E.numrows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.filename = NULL;
    E.statusmsg[0] = '\0';
    E.statusmsg_time = 0;
    E.syntax = NULL;

    // Set default values
    E.tab_stop = 8;  // Default tab stop
    E.quit_times = 3; // Default quit times

    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
    E.screenrows -= 2;
}

int main(int argc, char *argv[]) {
    enableRawMode();
    initEditor();

    // Load configuration if the file is provided
    if (argc >= 3) {
        loadConfig(argv[2]);
    } else if (argc >= 2) {
        editorOpen(argv[1]);
    }

    editorSetStatusMessage(
        "HELP: Ctrl-S = Save | Ctrl-Q = Quit | Ctrl-F = Find");

    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}

