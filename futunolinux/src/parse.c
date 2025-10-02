#include "sh.h"
/*
 * 構造体cmdポインタに、以下を割り当てておくinit処理的なもの
 *   cmdサイズをmallocする
 *   argc を０にする　→ argvに何個のトークンが入っているかのカウント
 *   argv -> トークンの先頭のポインタ配列
 *   capa = INIT_ARGV　->　argvの確保済みスロット数（足りなければ倍にrealloc)
 *   next = NULL -> パイプの右辺やリダイレクト先ファイルをぶら下げるため
 * 
 *  while(stdinからbufに収納した文字列が終端に達するまで処理を続ける)
 *  {
 *      while (p終端まで かつ pが空白だった場合は)
 *          ▶️　pの現在地に\0を入れてポインタを進める
 *      もし (pが空白でない かつ p が｜でない かつ pが>でもない場合)
 *      
 *      もし (pが空白でない かつ p が｜でない かつ pが>でもない場合))
 *      {
 *          もし (p終端まで かつ　(pが空白でない かつ p が｜でない かつ pが>でもない場合）)
 *          {
 *              もし (capaがargcよりも等しいか小さい場合は)
 *              {
 *                  ▶️ capa に２をかける
 *                  ▶️ argvに容量を足す
 *              }
 *          }
 *      }
 *  }
*/



// #define INIT_ARGV 8
/*
 * argv動的配列の初期容量を８にする理由：
 * 現実的な初期値である、8->16->32 と増やせば効率的である、メモリ負担が軽い（64bit環境）等
 * 
*/
// #define IDENT_CHAR_P(c) (!isspace((int)c) && ((c) != '|') && ((c) != '>'))

t_cmd *parse_command_line(char *p)
{
    t_cmd *cmd;

    
    cmd = xmalloc(sizeof(t_cmd));
    cmd->argc = 0;
    cmd->argv = xmalloc(sizeof(char*) * INIT_ARGV);
    cmd->capa = INIT_ARGV;
    cmd->next = NULL;
    while (*p) {
        //　空白はその場で\0を入れて飛ばす（直前トークンを確実に終端させる）
        while (*p && isspace((int)*p))
            *p++ = '\0';
        //  次の文字がトークン本文に含めないものなら抜ける（ここでは*p= '\0', '|', '>'の場合ということ）
        // 抜けた先はcmd->argv[cmd->argc]=NULL argvの終端処理のとこ
        if (! IDENT_CHAR_P(*p))
            break;
        // トークン先頭にいるのでargvに先頭ポインタを追加
        if (*p && IDENT_CHAR_P(*p)) {
            // 必要なら容量を拡張
            if (cmd->capa <= cmd->argc) {
                cmd->capa *= 2;
                cmd->argv = xrealloc(cmd->argv, sizeof(char *) * cmd->capa);
            }
            cmd->argv[cmd->argc] = p;
            cmd->argc++;
        }
        //トークン本文が続く限りpを進める
        while (*p && IDENT_CHAR_P(*p))
            p++;
    }
    /// argvの終端処理をする前にNULL終端用の１枠が確実にあるかのチェック、なければ１つだけ増やすという処理
    if (cmd->capa <= cmd->argc)
    {
        cmd->capa += 1;
        cmd->argv = xrealloc(cmd->argv, sizeof(char *) * cmd->capa);
    }
    // argvの終端処理
    cmd->argv[cmd->argc] = NULL;

    // もし'|'か'>'の場合は、右に再帰する
    if (*p == '|' || *p == '>') {
        if (cmd == NULL || cmd->argc == 0) 
            goto parse_error;
        cmd->next = parse_command_line(p + 1);
        if (cmd->next == NULL || cmd->next->argc == 0) 
            goto parse_error;
        if (*p == '>') 
        {
            if (cmd->next->argc != 1) 
                goto parse_error;
            // argcがフラグみたいな感じで使われてる
            cmd->next->argc = -1;
        }
        *p = '\0';
    }

    return cmd;

  parse_error:
    if (cmd) 
        free_cmd(cmd);
    return NULL;
}