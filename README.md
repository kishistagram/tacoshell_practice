# tacoshell_practice

由来 / ベース
**「ふつうのLinuxプログラミング」**のサンプル実装
https://github.com/aamine/stdlinux2-source/blob/master/sh2.c

を参考に、ファイル分割と最小限の改良を行ったもの

ファイル構成
include/
  └─ sh.h            // 共有ヘッダ

src/
  ├─ main.c          // エントリポイント / REPL (prompt)
  ├─ parse.c         // 入力行の解析 → 連結リスト(t_cmd)生成
  ├─ pipeline.c      // パイプライン生成・実行・待機
  ├─ redirect.c      // '>' のみの出力リダイレクト
  ├─ builtin_cmd.c   // ビルトインコマンド(cd, pwd, exit)
  ├─ builtins.c      // builtins_list[] の定義
  ├─ cmd_utils.c     // free_cmd / lookup_builtin など
  └─ utils.c         // xmalloc/xrealloc ほかユーティリティ


ビルドは付属の Makefile（make）で minishell が生成される

✅対応している機能

パイプ: cmd1 | cmd2 | ...

出力リダイレクト: > のみ（例: ... > out.txt）

ビルトイン: cd, pwd, exit

⚠️実装上の注意点 / 制限

Signal は未実装

そのため Ctrl-C / Ctrl-\ の挙動はシステムデフォルトに依存（プロンプトで終了することがある）

リダイレクトは > のみ対応（>>, <, << は未対応）

パーサは単方向連結リストベース

1 ノード = 1 コマンド（argv を保持）

末尾に**擬似ノード（argc == -1）**を置き、右隣のノードの argv[0] をリダイレクト先ファイルとして扱います

最小実装のため、クォート展開/変数展開/ワイルドカードなどは未対応です

使い方（例）
$ pwd
/path/to/project

$ ls | grep .c > list.txt
$ cat list.txt
main.c
parse.c
...

$ cd src
$ pwd
/path/to/project/src

$ exit

今後の拡張候補

>>, <, <<（heredoc）などの各種リダイレクト

Signal ハンドリング（プロンプト中の Ctrl-C をキャンセル扱いにする等）

パーサを AST/ノード種別（enum）に拡張して保守性向上

ビルトインの追加（export, unset, env など）