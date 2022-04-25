/* 【12月22日に出された課題】
 * ・課題内容
 * 指定された文字列の中の単語数をカウントして戻り値として返すプログラムを作成せよ。
 * スペースや改行, タブなどが現れるまで一単語とする。
 *
 * (参考ページ)
 * https://www.sgnet.co.jp/c/7-5.html
 */

#include <stdio.h>

// プロトタイプ宣言
int Myisalph(unsigned char cha);
int Mywc(char* inchar);

int main(void)
{
  // char型の配列宣言
  char string1[] = "  abt  090rb0 n adcei 987  qy9 ###23    99hi  :::  *    opm  ?><iub ";
  char string2[] = "ABJ pp * * ouu 9ioibhy :ouuh #$%%  yvjk a s d ))) &";
  char string3[] = "as dsttd uTIct $%%&'&( IKFTYUIUnu 3r iounipnp\\\\  \"\"";

  // printfで出力
  printf("string1 = \"%s\"\n", string1);
  printf("word count = %d\n\n", Mywc(string1));
  printf("string2 = \"%s\"\n", string2);
  printf("word count = %d\n\n", Mywc(string2));
  printf("string3 = \"%s\"\n", string3);
  printf("word count = %d\n\n", Mywc(string3));

  return 0;
}

// 空白の時はfalse, 文字の時はtrueを返す
int Myisalph(unsigned char cha)
{
  /* 「0x20」 = 空白
   * return (cha > 0x20) ? 1 : 0;
   */
  return cha > 0x20;
}

// 文字数のカウント処理
int Mywc(char* inchar)
{
  // 変数宣言
  int wc = 0;
  // char配列の要素数分繰り返し処理
  for(int cnt = 0; *(inchar + cnt) != '\0'; cnt++)
  {
    // char配列の要素の確認
    //printf("%08X %c\n", inchar + cnt, *(inchar + cnt));

    if (Myisalph(*(inchar + cnt)) == 1)
    {
      if (Myisalph(*(inchar + cnt + 1)) == 0)
      {
        wc += 1;
      }
    }
  }
  return wc;
}
