#include "sh.h"

extern char *program_name;

/*
 * コマンドの起動をする
 */

int invoke_commands(t_cmd *cmdhead)
{
    int st;
    int original_stdin = dup(0);
    int original_stdout = dup(1);

    exec_pipeline(cmdhead);
    st = wait_pipeline(cmdhead);
    close(0); 
    dup2(original_stdin, 0); 
    close(original_stdin);
    close(1); 
    dup2(original_stdout, 1); 
    close(original_stdout);

    return st;
}


void prompt(void)
{
    static char buf[LINEBUF_MAX];
    t_cmd *cmd;

    fprintf(stdout, "$ ");
    fflush(stdout);
    if (fgets(buf, LINEBUF_MAX, stdin) == NULL)
        exit(0);
    cmd = parse_command_line(buf);
    if (cmd == NULL) {
        fprintf(stderr, "%s: syntax error\n", program_name);
        return;
    }
    if (cmd->argc > 0)
        invoke_commands(cmd);
    free_cmd(cmd);
}



/* （main.c 流れ）
 * 
 * 標準出力にプロンプトとして"$ "を出す
 *  ⬇️
 * fflushはstdioの出力バッファをすぐ画面に出すための関数
 *  ⬇️
 * 　　　　　　注）fgetsはstdinのバッファに読み込みそのうち最大LINEBUF_MA -1文字を　bufにコピーし末尾に\0をつける、戻り値は正常ながbufのアドレス、エラーの場合はexitに進む
 *     　　　　　　minishellにおいてはreadlineを使う
 * もし(stdinからLINEBUF_MAX文字読み、エラーまたは最初の１文字も読めないEOFだったら）
 *      ▶️exit(0)する
 *  ⬇️
 * parse_command_line(buf)で、入力を解析する、解析結果のポインタをポインタcmdに入れとく
 *  ⬇️
 * もし (cmdがNULLだった場合）
 *      ▶️　（ターミナル入力の一番目の文字がプログラム名）：sytax errorとエラー出力に表示する
 *  ⬇️
 * もし (cmd構造体の中のargcをみて、０以上だった場合 )
 *      ▶️　コマンド起動
 *  ⬇️
 * 全ての処理が終わったらcmdをフリーする
 * 
*/

/*
 * プロンプト関数を実行し続ける
 * ⬇️
 * シェルの終了
*/
int main(int argc, char *argv[])
{
    (void)argc;
    program_name = argv[0];
    for (;;) {
        prompt();
    }
    exit(0);
}