// Copyright 2003-2010 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================

#include "base/basictypes.h"
#include "base/rand_util.h"
#include "omaha/base/debug.h"
#include "omaha/base/string.h"
#include "omaha/base/time.h"
#include "omaha/base/timer.h"
#include "omaha/testing/resource.h"
#include "omaha/testing/unit_test.h"

using std::string;

namespace omaha {

TEST(StringTest, IntToString) {
  ASSERT_STREQ(String_Int64ToString(0, 10), L"0");
  ASSERT_STREQ(String_Int64ToString(1, 10), L"1");
  ASSERT_STREQ(String_Int64ToString(-1, 10), L"-1");
  ASSERT_STREQ(String_Int64ToString(123456789, 10), L"123456789");
  ASSERT_STREQ(String_Int64ToString(-123456789, 10), L"-123456789");
  ASSERT_STREQ(String_Int64ToString(1234567890987654321, 10),
               L"1234567890987654321");
  ASSERT_STREQ(String_Int64ToString(-1234567890987654321, 10),
               L"-1234567890987654321");
  ASSERT_STREQ(String_Int64ToString(0xabcdef, 16), L"abcdef");
  ASSERT_STREQ(String_Int64ToString(0x101fff, 16), L"101fff");
  ASSERT_STREQ(String_Int64ToString(0x999999, 16), L"999999");
  ASSERT_STREQ(String_Int64ToString(0x0, 16), L"0");

  ASSERT_STREQ(String_Int64ToString(01234, 8), L"1234");
  ASSERT_STREQ(String_Int64ToString(0, 8), L"0");
  ASSERT_STREQ(String_Int64ToString(0777, 8), L"777");
  ASSERT_STREQ(String_Int64ToString(0123456, 8), L"123456");

  ASSERT_STREQ(String_Int64ToString(0, 2), L"0");
  ASSERT_STREQ(String_Int64ToString(0xf, 2), L"1111");
  ASSERT_STREQ(String_Int64ToString(0x5ad1, 2), L"101101011010001");
  ASSERT_STREQ(String_Int64ToString(-1, 2), L"-1");
}

TEST(StringTest, UintToString) {
  ASSERT_STREQ(String_Uint64ToString(0, 10), L"0");
  ASSERT_STREQ(String_Uint64ToString(1, 10), L"1");
  ASSERT_STREQ(String_Uint64ToString(123456789, 10), L"123456789");
  ASSERT_STREQ(String_Uint64ToString(1234567890987654321, 10),
               L"1234567890987654321");
  ASSERT_STREQ(String_Uint64ToString(18446744073709551615, 10),
               L"18446744073709551615");

  ASSERT_STREQ(String_Uint64ToString(0xabcdef, 16), L"abcdef");
  ASSERT_STREQ(String_Uint64ToString(0x101fff, 16), L"101fff");
  ASSERT_STREQ(String_Uint64ToString(0x999999, 16), L"999999");
  ASSERT_STREQ(String_Uint64ToString(0x0, 16), L"0");
  ASSERT_STREQ(String_Uint64ToString(0xffffffffffffffff, 16),
               L"ffffffffffffffff");

  ASSERT_STREQ(String_Uint64ToString(01234, 8), L"1234");
  ASSERT_STREQ(String_Uint64ToString(0, 8), L"0");
  ASSERT_STREQ(String_Uint64ToString(0777, 8), L"777");
  ASSERT_STREQ(String_Uint64ToString(0123456, 8), L"123456");

  ASSERT_STREQ(String_Uint64ToString(0, 2), L"0");
  ASSERT_STREQ(String_Uint64ToString(0xf, 2), L"1111");
  ASSERT_STREQ(String_Uint64ToString(0x5ad1, 2), L"101101011010001");
}

TEST(StringTest, DoubleToString) {
  ASSERT_STREQ(String_DoubleToString(1.234, 1), L"1.2");
  ASSERT_STREQ(String_DoubleToString(0.0, 0), L"0");
  ASSERT_STREQ(String_DoubleToString(0.0, 2), L"0.00");
  ASSERT_STREQ(String_DoubleToString(199.234, 2), L"199.23");
  ASSERT_STREQ(String_DoubleToString(-199.234, 2), L"-199.23");
  ASSERT_STREQ(String_DoubleToString(199.23490776, 5), L"199.23490");
  ASSERT_STREQ(String_DoubleToString(-1.0001, 1), L"-1.0");
  ASSERT_STREQ(String_DoubleToString(123456789.987654321, 3), L"123456789.987");
}

TEST(StringTest, StrNCpy) {
  const TCHAR * str1 = L"test str 1234";
  const TCHAR * str2 = L"test str 12";
  const TCHAR * str3 = L"Test StR 1234";

  // check case sensitive
  ASSERT_TRUE(0 == String_StrNCmp(str1, str2, 10, false));
  ASSERT_TRUE(0 == String_StrNCmp(str1, str2, 11, false));

  // check case in-sensitive
  ASSERT_TRUE(0 == String_StrNCmp(str2, str3, 10, true));
  ASSERT_TRUE(0 == String_StrNCmp(str2, str3, 11, true));
}

TEST(StringTest, StartsWith) {
  ASSERT_TRUE(String_StartsWith(L"", L"", false));
  ASSERT_TRUE(String_StartsWith(L"Joe", L"", false));
  ASSERT_TRUE(String_StartsWith(L"Joe", L"J", false));
  ASSERT_TRUE(String_StartsWith(L"Joe\\", L"J", false));
  ASSERT_TRUE(String_StartsWith(L"Joe", L"Joe", false));
  ASSERT_TRUE(String_StartsWith(L"The quick brown fox", L"The quic", false));
  ASSERT_FALSE(String_StartsWith(L"", L"J", false));
  ASSERT_FALSE(String_StartsWith(L"Joe", L"Joe2", false));
  ASSERT_FALSE(String_StartsWith(L"The quick brown fox", L"The quiC", false));

  ASSERT_TRUE(String_StartsWith(L"", L"", true));
  ASSERT_TRUE(String_StartsWith(L"Joe", L"j", true));
  ASSERT_TRUE(String_StartsWith(L"The quick brown fox", L"The quiC", true));
}

TEST(StringTest, StartsWithA) {
  ASSERT_TRUE(String_StartsWithA("", "", false));
  ASSERT_TRUE(String_StartsWithA("Joe", "", false));
  ASSERT_TRUE(String_StartsWithA("Joe", "J", false));
  ASSERT_TRUE(String_StartsWithA("Joe\\", "J", false));
  ASSERT_TRUE(String_StartsWithA("Joe", "Joe", false));
  ASSERT_TRUE(String_StartsWithA("The quick brown fox", "The quic", false));
  ASSERT_FALSE(String_StartsWithA("", "J", false));
  ASSERT_FALSE(String_StartsWithA("Joe", "Joe2", false));
  ASSERT_FALSE(String_StartsWithA("The quick brown fox", "The quiC", false));

  ASSERT_TRUE(String_StartsWithA("", "", true));
  ASSERT_TRUE(String_StartsWithA("Joe", "j", true));
  ASSERT_TRUE(String_StartsWithA("The quick brown fox", "The quiC", true));
}

TEST(StringTest, EndsWith) {
  // Case sensitive

  // Empty suffix
  ASSERT_TRUE(String_EndsWith(L"", L"", false));
  ASSERT_TRUE(String_EndsWith(L"Joe", L"", false));

  // Partial suffix
  ASSERT_TRUE(String_EndsWith(L"Joe", L"e", false));
  ASSERT_TRUE(String_EndsWith(L"Joe\\", L"\\", false));
  ASSERT_TRUE(String_EndsWith(L"The quick brown fox", L"n fox", false));

  // Suffix == String
  ASSERT_TRUE(String_EndsWith(L"Joe", L"Joe", false));
  ASSERT_TRUE(String_EndsWith(L"The quick brown fox",
                              L"The quick brown fox",
                              false));

  // Fail cases
  ASSERT_FALSE(String_EndsWith(L"", L"J", false));
  ASSERT_FALSE(String_EndsWith(L"Joe", L"Joe2", false));
  ASSERT_FALSE(String_EndsWith(L"Joe", L"2Joe", false));
  ASSERT_FALSE(String_EndsWith(L"The quick brown fox", L"n foX", false));

  // Check case insensitive

  // Empty suffix
  ASSERT_TRUE(String_EndsWith(L"", L"", true));
  ASSERT_TRUE(String_EndsWith(L"Joe", L"", true));

  // Partial suffix
  ASSERT_TRUE(String_EndsWith(L"Joe", L"E", true));
  ASSERT_TRUE(String_EndsWith(L"The quick brown fox", L"n FOX", true));

  // Suffix == String
  ASSERT_TRUE(String_EndsWith(L"Joe", L"JOE", true));
  ASSERT_TRUE(String_EndsWith(L"The quick brown fox",
                              L"The quick brown FOX",
                              true));

  // Fail cases
  ASSERT_FALSE(String_EndsWith(L"The quick brown fox", L"s", true));
  ASSERT_FALSE(String_EndsWith(L"The quick brown fox", L"Xs", true));
  ASSERT_FALSE(String_EndsWith(L"The quick brown fox", L"the brown foX", true));
}


#if 0
static const struct {
  const char *ansi;
  const TCHAR *wide;
  UINT cp;
} kAnsi2WideTests[] = {
  { "\xc8\xae\xc1\xbe", L"\x72ac\x8f86", CP_GB2312},
  { "\xa5\x69\xb1\x4e\xc2\xb2\xc5\xe9",
    L"\x53ef\x5c07\x7c21\x9ad4", CP_BIG5},
  { "\xE7\xBC\x96\xE4\xB8\x8B", L"\x7f16\x4e0b", CP_UTF8},
  { "ascii", L"ascii", CP_GB2312},
  { "\x3C\x20\xE7\xBC\x96", L"\x003c\x0020\x00E7\x00BC\x0096", 0 },
};

bool TestAnsiToWideString() {
  for (size_t i = 0; i < arraysize(kAnsi2WideTests); ++i) {
    CStringW out;
    if (kAnsi2WideTests[i].cp == 0) {
      out = AnsiToWideString(kAnsi2WideTests[i].ansi,
                             strlen(kAnsi2WideTests[i].ansi));
    } else {
      AnsiToWideString(kAnsi2WideTests[i].ansi,
                       strlen(kAnsi2WideTests[i].ansi),
                       kAnsi2WideTests[i].cp, &out);
    }
    CHK(out == kAnsi2WideTests[i].wide);
  }
  return true;
}
#endif

// Test international strings.
TEST(StringTest, International) {
  CString tabs_by_lang[] = {
      _T("Web    Prente    Groepe    Gids    "),                    // Afrikaans
      _T("Web    Fotografitë    Grupet    Drejtoriumi    "),        // Albanian
      // Amharic is missing, that doesn't show in normal windows fonts
      _T("ويب    صور    مجموعات    الدليل     "),                    // Arabic
      _T("Web   Şəkillər   Qruplar   Qovluq   "),                   // Azerbaijani
      _T("Web   Irudiak   Taldeak   Direktorioa   "),               // Basque
      _T("Ўэб    Малюнкі    Групы    Каталёг    "),                 // Belarusian
      _T("Antorjal    Chitraboli    Gosthi    Bishoy-Talika    "),  // Bengali
      _T("MakarJal    Chhaya    Jerow    Nirdeshika    "),          // Bihari
      _T("Veb   Imeges   Gruoops   Durectury "),                    // Bork
      _T("Internet    Slike    Grupe    Katalog    "),              // Bosnian
      _T("Gwiad    Skeudennoù    Strolladoù    Roll    "),          // Breton
      _T("Мрежата    Изображения    Групи    Директория    "),      // Bulgarian
      _T("Web    Imatges    Grups    Directori    "),               // Catalan
      _T("所有网站    图像    网上论坛    网页目录    "),                            // Chinese Simplified
      _T("所有網頁    圖片    網上論壇    網頁目錄     "),                            // Chinese Traditional
      _T("Web    Slike    Grupe    Imenik    "),                    // Croatian
      _T("Web    Obrázky    Skupiny    Adresář    "),               // Czech
      _T("Nettet    Billeder    Grupper    Katalog    "),           // Danish
      _T("Het Internet    Afbeeldingen    Discussiegroepen    Gids    "),  // Dutch
      _T("Web    Images    Gwoups    Diwectowy    "),               // Elmer
      _T("Web    Images    Groups    News    Froogle    more »"),   // English
      _T("TTT    Bildoj    Grupoj    Katalogo     "),               // Esperanto
      _T("Veeb    Pildid    Grupid    Kataloog    "),               // Estonian
      _T("Netið    Myndir    Bólkar    Øki    "),                   // Faroese
      _T("Web    Mga Larawan    Mga Grupo    Direktoryo    "),      // Filipino
      _T("Web    Kuvat    Keskusteluryhmät    Hakemisto    "),      // Finnish
      _T("Web    Images    Groupes    Annuaire    Actualités    "),  // French
      _T("Web   Printsjes   Diskusjegroepen   Directory   "),       // Frisian
      _T("Web    Imaxes    Grupos    Directorio    "),              // Galician
      _T("ინტერნეტი   სურათები   ჯგუფები   კატალოგი   "),                      // Georgian
      _T("Web    Bilder    Groups    Verzeichnis    News    "),     // German
      _T("Ιστός    Eικόνες    Ομάδες    Κατάλογος    "),            // Greek
      _T("Ñanduti   Ta'anga   Atypy   Sãmbyhypy "),                 // Guarani
      _T("jalu    Chhabi    Sangathan    Shabdakosh    "),          // Gujarati
      _T("n0rM4L s33rCh    1|\\/|4935    6r00pZ    d1r3c70rY    "),  // Hacker
      _T("אתרים ברשת    תמונות    קבוצות דיון    מדריך האתרים     "),  // Hebrew
      _T("वेब    छवियाँ    समूह    निर्देशिका    "),                             // Hindi
      _T("Web    Képek    Csoportok    Címtár    "),                // Hungarian
      _T("Vefur    Myndir    Hópar    Flokkar    "),                // Icelandic
      _T("Web    Gambar    Grup    Direktori    "),                 // Indonesian
      _T("Web    Imagines    Gruppos    Catalogo  "),               // Interlingua
      _T("An Gréasán    Íomhánna    Grúpaí    Eolaire    "),        // Irish
      _T("Web    Immagini    Gruppi    Directory    News Novità!    "),  // Italian
      _T("ウェブ    イメージ    グループ    ディレクトリ    "),                        // Japanese
      _T("Web    Gambar - gambar    Paguyuban    Bagian    "),      // Javanese
      _T("antharajAla    chitragaLu    gumpugaLu    Huduku vibhaagagaLu    "),  // Kannada
      _T("Daqmey pat    naghmey beQ    ghommey    mem    "),        // Klingon
      _T("웹 문서    이미지    뉴스그룹    디렉토리    "),                            // Klingon
      _T("Желе   Суроттор   Группалар   Тизме   "),                 // Kyrgyz
      _T("Tela   Imagines   Circuli   Index "),                     // Latin
      _T("Internets    Attēli    Vēstkopas    Katalogs"),           // Latvian
      _T("Internetas    Vaizdai    Grupės    Katalogas    "),       // Lithuanian
      _T("Мрежа    Слики    Групи    Директориум    "),             // Macedonian
      _T("Jaringan    Imej    Kumpulan    Direktori    "),          // Malay
      _T("വെബ്    ചിത്രങ്ങള്    സംഘങ്ങള്    ഡയറക്ടറി    "),                     // Malayalam
      _T("Web    Stampi    Gruppi    Direttorju    "),              // Maltese
      _T("वेबशोध    चित्रशोध    ग्रूप्स    डिरेक्टरी    "),                          // Marathi
      _T("वेब    तस्वीर    समूह    डाइरेक्टरी    "),                            // Nepali
      _T("Nett    Bilder    Grupper    Katalog    "),               // Norwegian
      _T("Veven    Bilete    Grupper    Katalog    "),              // Norwegian (Nynorsk)
      _T("Ret    Imatges    Grops    Directori    "),               // Occitan
      _T("web   chitra   goSThi   prasanga tAlikA "),               // Oriya
      _T("وب    تصويرها    گروهها    فهرست     "),                  // Persian
      _T("ebway    imagesyay    oupsgray    Irectoryday    "),      // P. Latin
      _T("WWW    Grafika    Grupy dyskusyjne    Katalog   "),       // Polish
      _T("Web    Imagens    Grupos    Diretório    "),              // Potruguese (Brazil)
      _T("Web    Imagens    Grupos    Directório  "),               // Potruguese (Portugal)
      _T("Web/Zaal    Tasveraan    Gutt    Directory    "),         // Punjabi
      _T("Web    Imagini    Grupuri    Director    "),              // Romanian
      _T("Веб    Картинки    Группы    Каталог  "),                 // Russian
      _T("Lìon    Dealbhan    Cuantail    Eòlaire    "),            // Scots Gaelic
      _T("Интернет    Слике    Групе    Каталог    "),              // Serbian
      _T("Internet    Slike    Grupe    Spisak    "),               // Serbo-Croatian
      _T("Web   Ponahalo   Dihlopha   Tshupetso "),                 // Sesotho
      _T("WEB    Roopa    Kandayam    Namawaliya    "),             // Sinhalese
      _T("Web    Obrázky    Skupiny    Katalóg    "),               // Slovak
      _T("Internet    Slike    Skupine    Imenik    "),             // Slovenian
      _T("La Web    Imágenes    Grupos    Directorio    News ¡Nuevo!    "),  // Spanish
      _T("Web   Gambar   Grup   Direktori "),                       // Sudanese
      _T("Mtandao    Picha    Vikundi    Orodha    "),              // Swahili
      _T("Nätet    Bilder    Grupper    Kategori    "),             // Swedish
      _T("வலை    படங்கள்    குழுக்கள்    விபரக்கோவை    "),          // Tamil
      _T("వెబ్    చిత్రాలు    సమూహములు    darshini    "),                        // Telugu
      _T("เว็บ    รูปภาพ    กลุ่มข่าว    สารบบเว็บ    "),                            // Thai
      // Tigrinya is missing, that doesn't show in normal windows fonts
      _T("Web    Grafikler    Gruplar    Dizin    "),               // Turkish
      _T("Web   Suratlar   Toparlar   Düzine "),                    // Turkmen
      _T("tintan   Nfonyin   Akuokuo   Krataa nhwemu "),            // Twi
      _T("Веб    Зображення    Групи    Каталог    "),              // Ukrainian
      _T("ويب    تصاوير    گروہ    فہرست         ")                           // Urdu
      _T("To'r    Tasvirlar    Gruppalar    Papka    "),            // Uzbek
      _T("Internet    Hình Ảnh    Nhóm    Thư Mục    "),            // Vietnamese
      _T("Y We    Lluniau    Grwpiau    Cyfeiriadur    "),          // Welsh
      _T("Web   Imifanekiso   Amaqela   Isilawuli "),               // Xhosa
      _T("װעב    בילדער    גרופּעס    פּאַפּקע     "),                  // Yiddish
      _T("I-web   Izithombe   Amaqembu   Uhlu lwamafayela   "),     // Zulu
  };

  int i = 0;
  for(i = 0; i < arraysize(tabs_by_lang); ++i) {
    // Get the cannonical lower version with ::CharLower
    CString true_lower(tabs_by_lang[i]);
    ::CharLower(true_lower.GetBuffer());
    true_lower.ReleaseBuffer();

    // Get the lower version with String_ToLower,
    CString low_temp(tabs_by_lang[i]);
    String_ToLower(low_temp.GetBuffer());
    low_temp.ReleaseBuffer();

    // make sure they match
    ASSERT_STREQ(low_temp, true_lower);

    // Now make sure they match letter by letter
    for(int j = 0; j < tabs_by_lang[i].GetLength(); ++j) {
      TCHAR cur_char = tabs_by_lang[i].GetAt(j);

      TCHAR low1 = static_cast<TCHAR>(String_ToLowerChar(cur_char));

      ASSERT_EQ(low1, true_lower.GetAt(j));
      ASSERT_EQ(Char_ToLower(cur_char), true_lower.GetAt(j));

      // Check the Ansi version if applicable
      if (cur_char < 128)
        ASSERT_EQ(String_ToLowerChar(static_cast<char>(cur_char)),
                  true_lower.GetAt(j));
    }

    // Test out the CString conversion
    CString temp(tabs_by_lang[i]);
    MakeLowerCString(temp);
    ASSERT_STREQ(temp, true_lower);

    // Test out the fast version
    temp = tabs_by_lang[i];
    String_FastToLower(temp.GetBuffer());
    temp.ReleaseBuffer();

    ASSERT_STREQ(temp, true_lower);
  }
}

TEST(StringTest, ReplaceCString) {
  CString t;
  t = _T("a a a b ");
  ReplaceCString(t, _T("a"), 1, _T("d"), 1, 5);
  ASSERT_STREQ(_T("d d d b "), t);

  t = _T("a a a b ");
  ReplaceCString(t, _T("b"), 1, _T("d"), 1, 5);
  ASSERT_STREQ(_T("a a a d "), t);

  t = _T("a a a b ");
  ReplaceCString(t, _T("a"), 1, _T("d"), 1, 1);
  ASSERT_STREQ(_T("d a a b "), t);

  t = _T("a a a b ");
  ReplaceCString(t, _T("a"), 1, _T("dd"), 2, 5);
  ASSERT_STREQ(_T("dd dd dd b "), t);

  ReplaceCString(t, _T("dd"), 2, _T("dddd"), 4, 5);
  ASSERT_STREQ(_T("dddd dddd dddd b "), t);

  ReplaceCString(t, _T("dd"), 2, _T("dddd"), 4, 5);
  ASSERT_STREQ(_T("dddddddd dddddddd dddddd b "), t);

  ReplaceCString(t, _T("dddddddd"), 8, _T("dddd"), 4, 2);
  ASSERT_STREQ(_T("dddd dddd dddddd b "), t);

  ReplaceCString(t, _T("d"), 1, _T("a"), 1, 2);
  ASSERT_STREQ(_T("aadd dddd dddddd b "), t);

  ReplaceCString(t, _T("d d"), 3, _T("c"), 1, 2);
  ASSERT_STREQ(_T("aadcddcddddd b "), t);

  ReplaceCString(t, _T("c"), 1, _T("1234567890"), 10, 2);
  ASSERT_STREQ(_T("aad1234567890dd1234567890ddddd b "), t);

  ReplaceCString(t, _T("1"), 1, _T("1234567890"), 10, 2);
  ASSERT_STREQ(_T("aad1234567890234567890dd1234567890234567890ddddd b "), t);

  ReplaceCString(t, _T("1234567890"), 10, _T(""), 0, 2);
  ASSERT_STREQ(_T("aad234567890dd234567890ddddd b "), t);

  t = _T("a aa aa b ");
  ReplaceCString(t, _T("aa"), 2, _T("b"), 1, 5);
  ASSERT_STREQ(_T("a b b b "), t);

  t = _T("moo a aa aa b ");
  ReplaceCString(t, _T("aa"), 2, _T("b"), 1, 5);
  ASSERT_STREQ(_T("moo a b b b "), t);

  // Time to test some big strings
  int test_sizes[] = {200, 500, 900, 10000};

  int i;
  for(i = 0; i < arraysize(test_sizes); ++i) {
    CString in, out;
    for(int j = 0; j < test_sizes[i]; ++j) {
      in += L'a';
      out += _T("bb");
    }
    CString bak_in(in);

    // Make it a bit bigger
    int times = ReplaceCString(in, _T("a"), 1, _T("bb"), 2, kRepMax);
    ASSERT_EQ(times, test_sizes[i]);
    ASSERT_EQ(out, in);

    // Make it bigger still
    times = ReplaceCString(in, _T("bb"), 2, _T("ccc"), 3, kRepMax);
    ASSERT_EQ(times, test_sizes[i]);

    // Same size swap
    times = ReplaceCString(in, _T("c"), 1, _T("d"), 1, kRepMax);
    ASSERT_EQ(times, test_sizes[i] * 3);

    // Make it smaller again
    times = ReplaceCString(in, _T("ddd"), 3, _T("a"), 1, kRepMax);
    ASSERT_EQ(times, test_sizes[i]);
    ASSERT_EQ(bak_in, in);
  }
}

TEST(StringTest, GetAbsoluteUri) {
  ASSERT_STREQ(GetAbsoluteUri(L"http://www.google.com"),
               L"http://www.google.com/");
  ASSERT_STREQ(GetAbsoluteUri(L"http://www.google.com/"),
               L"http://www.google.com/");
  ASSERT_STREQ(GetAbsoluteUri(L"http://www.google.com//"),
               L"http://www.google.com/");
  ASSERT_STREQ(GetAbsoluteUri(L"http://www.google.com/test"),
               L"http://www.google.com/test");
}

// IsSpaceA1 is much faster without the cache clearing (which is what happends
// in release mode)
// IsSpaceA1 is roughly the same speed as IsSpaceA2 with cache clearing (in
// debug mode)
// IsSpaceA3 is always much slower

static const byte spacesA[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  // 0-9
  1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  // 10-19
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 20-29
  0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  // 30-39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40-49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 50-59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 60-69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 70-79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 80-89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 90-99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 100-109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 110-119
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 120-129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 130-139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 140-149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 150-159
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 160-169
};

bool IsSpaceA1(char c) {
  return spacesA[c] == 1;
}

bool IsSpaceA2(char c) {
  // return (c==32);
  // if (c>32) { return 0; }
  // most characters >32, check first for that case
  if (c>32 && c!=160) { return 0; }
  if (c==32) { return 1; }
  if (c>=9&&c<=13) return 1; else return 0;
}

bool IsSpaceA3(char c) {
  WORD result;
  if (GetStringTypeA(0, CT_CTYPE1, &c, 1, &result)) {
    return (0 != (result & C1_SPACE));
  }
  return false;
}

void TestIsSpace (const char *s) {
    ASSERT_TRUE(s);

    Timer t1 (false);
    Timer t2 (false);
    Timer t3 (false);

    // int iterations = 10000;
    size_t iterations = 100;
    size_t len = strlen (s);

    // used to try to clear the processor cache
    const size_t dlen = 100000;
    char* rand_bytes = new char [dlen];
    RandBytes(rand_bytes, sizeof(*rand_bytes) * dlen);

    size_t num_spaces = 0;
    size_t n = iterations * len;
    for (size_t i = 0; i < iterations; i++) {
        t1.Start();
        for (size_t j = 0; j < len; j++) {
            num_spaces += IsSpaceA1 (s[j]);
        }
        t1.Stop();
        // this cache clearing code gets optimized out in release mode
        size_t d2 = 0;
        for (size_t j = 0; j < dlen; j++) { d2 += rand_bytes[j]; }
    }

    num_spaces = 0;
    for (size_t i = 0; i < iterations; i++) {
        t2.Start();
        for (size_t j = 0; j < len; j++) {
            num_spaces += IsSpaceA2 (s[j]);
        }
        t2.Stop();
        size_t d2 = 0;
        for (size_t j = 0; j < dlen; j++) { d2 += rand_bytes[j]; }
    }

    num_spaces = 0;
    for (size_t i = 0; i < iterations; i++) {
        t3.Start();
        for (size_t j = 0; j < len; j++) {
            num_spaces += IsSpaceA3 (s[j]);
        }
        t3.Stop();
        size_t d2 = 0;
        for (size_t j = 0; j < dlen; j++) { d2 += rand_bytes[j]; }
    }

    delete[] rand_bytes;
}

TEST(StringTest, IsSpace) {
  TestIsSpace("banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie banana pie");
  TestIsSpace("sdlfhdkgheorutsgj sdlj aoi oaj gldjg opre gdsfjng oate yhdnv ;zsj fpoe v;kjae hgpaieh dajlgn aegh avn WEIf h9243y 9814cu 902t7 9[-32 [O8W759 RC90817 V9pDAHc n( ny(7LKFJAOISF *&^*^%$$%#*&^(*_*)_^& 67% 796%&$*^$ 8)6 (^ 08&^ )*^ 9-7=90z& +(^ )^* %9%4386 $& (& &+ 7- &(_* ");
}

TEST(StringTest, IsDigit) {
  ASSERT_TRUE(String_IsDigit('0'));
  ASSERT_TRUE(String_IsDigit('1'));
  ASSERT_TRUE(String_IsDigit('2'));
  ASSERT_TRUE(String_IsDigit('3'));
  ASSERT_TRUE(String_IsDigit('4'));
  ASSERT_TRUE(String_IsDigit('5'));
  ASSERT_TRUE(String_IsDigit('6'));
  ASSERT_TRUE(String_IsDigit('7'));
  ASSERT_TRUE(String_IsDigit('8'));
  ASSERT_TRUE(String_IsDigit('9'));
  ASSERT_FALSE(String_IsDigit('a'));
  ASSERT_FALSE(String_IsDigit('b'));
  ASSERT_FALSE(String_IsDigit('z'));
  ASSERT_FALSE(String_IsDigit('A'));
  ASSERT_FALSE(String_IsDigit(' '));
  ASSERT_FALSE(String_IsDigit('#'));
}

TEST(StringTest, IsUpper) {
  ASSERT_FALSE(String_IsUpper('0'));
  ASSERT_FALSE(String_IsUpper(' '));
  ASSERT_FALSE(String_IsUpper('#'));
  ASSERT_FALSE(String_IsUpper('a'));
  ASSERT_FALSE(String_IsUpper('z'));
  ASSERT_TRUE(String_IsUpper('A'));
  ASSERT_TRUE(String_IsUpper('B'));
  ASSERT_TRUE(String_IsUpper('C'));
  ASSERT_TRUE(String_IsUpper('D'));
  ASSERT_TRUE(String_IsUpper('H'));
  ASSERT_TRUE(String_IsUpper('Y'));
  ASSERT_TRUE(String_IsUpper('Z'));
}

TEST(StringTest, StringToDouble) {
  ASSERT_DOUBLE_EQ(String_StringToDouble(L"625"), 625);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L"-625"), -625);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L"-6.25"), -6.25);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L"6.25"), 6.25);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L"0.00"), 0);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L" 55.1"), 55.1);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L" 55.001"), 55.001);
  ASSERT_DOUBLE_EQ(String_StringToDouble(L"  1.001"), 1.001);
}

TEST(StringTest, StringToInt) {
  ASSERT_EQ(String_StringToInt(L"625"), 625);
  ASSERT_EQ(String_StringToInt(L"6"), 6);
  ASSERT_EQ(String_StringToInt(L"0"), 0);
  ASSERT_EQ(String_StringToInt(L" 122"), 122);
  ASSERT_EQ(String_StringToInt(L"a"), 0);
  ASSERT_EQ(String_StringToInt(L" a"), 0);
}

TEST(StringTest, StringToInt64) {
  ASSERT_EQ(String_StringToInt64(L"119600064000000000"),
            119600064000000000uI64);
  ASSERT_EQ(String_StringToInt64(L" 119600064000000000"),
            119600064000000000uI64);
  ASSERT_EQ(String_StringToInt64(L"625"), 625);
  ASSERT_EQ(String_StringToInt64(L"6"), 6);
  ASSERT_EQ(String_StringToInt64(L"0"), 0);
  ASSERT_EQ(String_StringToInt64(L" 122"), 122);
  ASSERT_EQ(String_StringToInt64(L"a"), 0);
  ASSERT_EQ(String_StringToInt64(L" a"), 0);
}

void TestEndWithChar(const TCHAR *s, char c, const TCHAR *expected) {
  ASSERT_TRUE(expected);
  ASSERT_TRUE(s);

  TCHAR buf[5000];
  _tcscpy(buf, s);
  String_EndWithChar(buf, c);
  ASSERT_STREQ(buf, expected);
}

TEST(StringTest, EndWithChar) {
  TestEndWithChar(L"", L'a', L"a");
  TestEndWithChar(L"", L'\\', L"\\");
  TestEndWithChar(L"a", L'a', L"a");
  TestEndWithChar(L"a", L'b', L"ab");
  TestEndWithChar(L"abcdefghij", L'a', L"abcdefghija");
  TestEndWithChar(L"abcdefghij", L'\\', L"abcdefghij\\");
}

TEST(StringTest, HexDigitToInt) {
  ASSERT_EQ(HexDigitToInt(L'0'), 0);
  ASSERT_EQ(HexDigitToInt(L'1'), 1);
  ASSERT_EQ(HexDigitToInt(L'2'), 2);
  ASSERT_EQ(HexDigitToInt(L'3'), 3);
  ASSERT_EQ(HexDigitToInt(L'4'), 4);
  ASSERT_EQ(HexDigitToInt(L'5'), 5);
  ASSERT_EQ(HexDigitToInt(L'6'), 6);
  ASSERT_EQ(HexDigitToInt(L'7'), 7);
  ASSERT_EQ(HexDigitToInt(L'8'), 8);
  ASSERT_EQ(HexDigitToInt(L'9'), 9);
  ASSERT_EQ(HexDigitToInt(L'A'), 10);
  ASSERT_EQ(HexDigitToInt(L'a'), 10);
  ASSERT_EQ(HexDigitToInt(L'B'), 11);
  ASSERT_EQ(HexDigitToInt(L'b'), 11);
  ASSERT_EQ(HexDigitToInt(L'C'), 12);
  ASSERT_EQ(HexDigitToInt(L'c'), 12);
  ASSERT_EQ(HexDigitToInt(L'D'), 13);
  ASSERT_EQ(HexDigitToInt(L'd'), 13);
  ASSERT_EQ(HexDigitToInt(L'E'), 14);
  ASSERT_EQ(HexDigitToInt(L'e'), 14);
  ASSERT_EQ(HexDigitToInt(L'F'), 15);
  ASSERT_EQ(HexDigitToInt(L'f'), 15);
}

TEST(StringTest, IsHexDigit) {
  ASSERT_TRUE(IsHexDigit(L'0'));
  ASSERT_TRUE(IsHexDigit(L'1'));
  ASSERT_TRUE(IsHexDigit(L'2'));
  ASSERT_TRUE(IsHexDigit(L'3'));
  ASSERT_TRUE(IsHexDigit(L'4'));
  ASSERT_TRUE(IsHexDigit(L'5'));
  ASSERT_TRUE(IsHexDigit(L'6'));
  ASSERT_TRUE(IsHexDigit(L'7'));
  ASSERT_TRUE(IsHexDigit(L'8'));
  ASSERT_TRUE(IsHexDigit(L'9'));
  ASSERT_TRUE(IsHexDigit(L'a'));
  ASSERT_TRUE(IsHexDigit(L'A'));
  ASSERT_TRUE(IsHexDigit(L'b'));
  ASSERT_TRUE(IsHexDigit(L'B'));
  ASSERT_TRUE(IsHexDigit(L'c'));
  ASSERT_TRUE(IsHexDigit(L'C'));
  ASSERT_TRUE(IsHexDigit(L'd'));
  ASSERT_TRUE(IsHexDigit(L'D'));
  ASSERT_TRUE(IsHexDigit(L'e'));
  ASSERT_TRUE(IsHexDigit(L'E'));
  ASSERT_TRUE(IsHexDigit(L'f'));
  ASSERT_TRUE(IsHexDigit(L'F'));

  for(TCHAR digit = static_cast<TCHAR>(127); digit < 10000; ++digit) {
    ASSERT_FALSE(IsHexDigit(digit));
  }
}

TEST(StringTest, WideToAnsiDirect) {
  CString temp_convert;
  ASSERT_STREQ("", WideToAnsiDirect(_T("")));
  ASSERT_STREQ("a", WideToAnsiDirect(_T("a")));
  ASSERT_STREQ("moon doggy", WideToAnsiDirect(_T("moon doggy")));

  // Generate a string of all characters 0-255.
  const int kNumChars = 256;
  TCHAR nasty_chars[kNumChars];
  for (int i = 0; i < kNumChars; ++i) {
    nasty_chars[i] = static_cast<TCHAR>(i);
  }
  CString temp(nasty_chars, kNumChars);

  // Convert it and make sure it matches.
  CStringA out = WideToAnsiDirect(temp);
  ASSERT_EQ(out.GetLength(), kNumChars);
  for (int i = 0; i < kNumChars; ++i) {
    ASSERT_EQ(static_cast<unsigned char>(nasty_chars[i]),
              static_cast<unsigned char>(out.GetAt(i)));
  }
}

TEST(StringTest, Base64) {
  struct {
    const char* binary;
    const char* base64;
  } test_data[] = {
    "",                                "",
    "what",                            "d2hhdA==",
    "what will print out",             "d2hhdCB3aWxsIHByaW50IG91dA==",
    "foobar",                          "Zm9vYmFy",
    "a man, a plan, a canal: panama!", "YSBtYW4sIGEgcGxhbiwgYSBjYW5hbDogcGFuYW1hIQ==",    // NOLINT
  };

  for (size_t i = 0; i != arraysize(test_data); i++) {
    CStringA test_e;
    Base64Escape(test_data[i].binary,
                 strlen(test_data[i].binary),
                 &test_e,
                 true);
    EXPECT_STREQ(test_e, test_data[i].base64);

    CStringA test_d;
    ASSERT_GE(Base64Unescape(test_e, &test_d), 0);
    EXPECT_STREQ(test_d, test_data[i].binary);
  }
}

TEST(StringTest, Base64Unescape) {
  CStringA input("AAAAAAAA");
  CStringA output;
  ASSERT_EQ(Base64Unescape(input, &output), input.GetLength() * 3 / 4);
}

TEST(StringTest, TextToLinesAndBack) {
  const TCHAR sample_input[]  = L"Now is the time\r\nfor all good men\r\nto come to the aid of their country";
  const TCHAR* sample_lines[] = { L"Now is the time", L"for all good men", L"to come to the aid of their country" };
  const TCHAR sample_output1[] = L"Now is the time\nfor all good men\nto come to the aid of their country\n";
  const TCHAR sample_output2[] = L"Now is the timefor all good mento come to the aid of their country";

  CString text_in(sample_input);
  std::vector<CString> lines;
  CString text_out;

  TextToLines(text_in, L"\r\n", &lines);
  ASSERT_EQ(lines.size(), 3);
  for (size_t i = 0; i < arraysize(sample_lines); ++i) {
    ASSERT_TRUE(0 == lines[i].Compare(sample_lines[i]));
  }
  LinesToText(lines, L"\n", &text_out);
  ASSERT_TRUE(0 == text_out.Compare(sample_output1));
  LinesToText(lines, L"", &text_out);
  ASSERT_TRUE(0 == text_out.Compare(sample_output2));
}

CString TrimStdString(const TCHAR* str) {
  CString s(str);
  TrimString(s, L" \t");
  return s;
}

TEST(StringTest, TrimString) {
  ASSERT_STREQ(L"abc", TrimStdString(L"abc"));
  ASSERT_STREQ(L"abc", TrimStdString(L" abc "));
  ASSERT_STREQ(L"a c", TrimStdString(L" a c  "));
  ASSERT_STREQ(L"abc", TrimStdString(L" \tabc\t "));
  ASSERT_STREQ(L"", TrimStdString(L""));
  ASSERT_STREQ(L"", TrimStdString(L"   "));
}

TEST(StringTest, EscapeUnescape) {
  CString original_str(_T("test <>\"#{}|\\^[]?%&/"));
  CString escaped_str;
  ASSERT_SUCCEEDED(StringEscape(original_str, true, &escaped_str));
  ASSERT_STREQ(escaped_str,
               _T("test%20%3C%3E%22%23%7B%7D%7C%5C%5E%5B%5D%3F%25%26%2F"));
  CString unescaped_str;
  ASSERT_SUCCEEDED(StringUnescape(escaped_str, &unescaped_str));
  ASSERT_STREQ(original_str, unescaped_str);

  original_str = _T("foo.test path?app=1");
  ASSERT_SUCCEEDED(StringEscape(original_str, false, &escaped_str));
  ASSERT_STREQ(escaped_str,
               _T("foo.test%20path?app=1"));
  ASSERT_SUCCEEDED(StringUnescape(escaped_str, &unescaped_str));
  ASSERT_STREQ(original_str, unescaped_str);
}

TEST(StringTest, ReplaceIgnoreCase) {
  EXPECT_STREQ(_T("AddddAddddAB"),
               String_ReplaceIgnoreCase(_T("ABCABCAB"), _T("bc"), _T("dddd")));
  EXPECT_STREQ(_T("AdAdAd"),
               String_ReplaceIgnoreCase(_T("ABCABCABC"), _T("bc"), _T("d")));
}

TEST(StringTest, String_StringToDecimalIntChecked) {
  int value = 0;

  // This code before the first valid case verifies that errno is properly
  // cleared and there are no dependencies on prior code.
  EXPECT_EQ(0, _set_errno(ERANGE));

  // Valid Cases
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T("935"), &value));
  EXPECT_EQ(value, 935);
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T("-935"), &value));
  EXPECT_EQ(value, -935);
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T("0"), &value));
  EXPECT_EQ(value, 0);
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T("2147483647"), &value));
  EXPECT_EQ(value, LONG_MAX);
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T("-2147483648"), &value));
  EXPECT_EQ(value, LONG_MIN);
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T(" 0"), &value));
  EXPECT_EQ(value, 0);

  // Failing Cases
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T(""), &value));
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("2147483648"), &value));
  EXPECT_EQ(value, LONG_MAX);
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("-2147483649"), &value));
  EXPECT_EQ(value, LONG_MIN);
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("0x935"), &value));
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("nine"), &value));
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("9nine"), &value));
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("nine9"), &value));

  // A valid case after an overflow verifies that this method clears errno.
  EXPECT_FALSE(String_StringToDecimalIntChecked(_T("2147483648"), &value));
  EXPECT_TRUE(String_StringToDecimalIntChecked(_T("935"), &value));
}
TEST(StringTest, String_StringToTristate) {
  Tristate value = TRISTATE_NONE;

  // Valid Cases
  EXPECT_TRUE(String_StringToTristate(_T("0"), &value));
  EXPECT_EQ(value, TRISTATE_FALSE);
  EXPECT_TRUE(String_StringToTristate(_T("1"), &value));
  EXPECT_EQ(value, TRISTATE_TRUE);
  EXPECT_TRUE(String_StringToTristate(_T("2"), &value));
  EXPECT_EQ(value, TRISTATE_NONE);

  // Invalid Cases
  EXPECT_FALSE(String_StringToTristate(_T("-1"), &value));
  EXPECT_FALSE(String_StringToTristate(_T("3"), &value));
  EXPECT_FALSE(String_StringToTristate(_T(""), &value));
}

TEST(StringTest, ParseNameValuePair) {
  CString name;
  CString value;

  // Valid Cases
  EXPECT_TRUE(ParseNameValuePair(_T("xx=yyzz"), _T('='), &name, &value));
  EXPECT_EQ(name, _T("xx"));
  EXPECT_EQ(value, _T("yyzz"));
  EXPECT_TRUE(ParseNameValuePair(_T("x=3?\\/\r\n "), _T('='), &name, &value));
  EXPECT_EQ(name, _T("x"));
  EXPECT_EQ(value, _T("3?\\/\r\n "));
  EXPECT_TRUE(ParseNameValuePair(_T("3?google"), _T('?'), &name, &value));
  EXPECT_EQ(name, _T("3"));
  EXPECT_EQ(value, _T("google"));

  // Invalid Cases
  EXPECT_FALSE(ParseNameValuePair(_T(""), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T(" "), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("="), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("x="), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("=y"), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("="), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("xxyyzz"), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("xx yyzz"), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("xx==yyzz"), _T('='), &name, &value));
  EXPECT_FALSE(ParseNameValuePair(_T("xx=yy=zz"), _T('='), &name, &value));
}

TEST(StringTest, SplitCommandLineInPlace) {
  const TCHAR * const test_long_paths[] = {
    _T("c:\\Program Files\\Google\\App\\App.exe"),
    _T("c:\\Program Files\\Google\\App\\Long Name App.exe"),
  };
  const TCHAR * const test_short_paths[] = {
    _T("notepad.exe"),
  };
  const TCHAR * const test_arguments[] = {
    _T("/a=\"some text\""),
    _T("/a /b /c"),
    _T(""),
  };
  TCHAR command_line[1024] = {};
  TCHAR* path = NULL;
  TCHAR* arguments = NULL;
  for (int ii = 0; ii < ARRAYSIZE(test_arguments) ; ++ii) {
    for (int jj = 0; jj < ARRAYSIZE(test_long_paths) ; ++jj) {
      _snwprintf_s(command_line, ARRAYSIZE(command_line), _TRUNCATE,
          _T("\"%s\" %s"), test_long_paths[jj], test_arguments[ii]);
      EXPECT_EQ(true, SplitCommandLineInPlace(command_line, &path, &arguments));
      EXPECT_STREQ(test_long_paths[jj], path);
      EXPECT_STREQ(test_arguments[ii], arguments);
    }
    for (int kk = 0; kk < ARRAYSIZE(test_short_paths) ; ++kk) {
      _snwprintf_s(command_line, ARRAYSIZE(command_line), _TRUNCATE,
          _T("%s %s"), test_short_paths[kk], test_arguments[ii]);
      EXPECT_EQ(true, SplitCommandLineInPlace(command_line, &path, &arguments));
      EXPECT_STREQ(test_short_paths[kk], path);
      EXPECT_STREQ(test_arguments[ii], arguments);
    }
  }
}

TEST(StringTest, ContainsOnlyAsciiChars) {
  CString test(_T("hello worlr"));
  ASSERT_TRUE(ContainsOnlyAsciiChars(test));

  TCHAR non_ascii[] = {0x4345, 0x1234, 0x2000};
  ASSERT_FALSE(ContainsOnlyAsciiChars(non_ascii));
}
TEST(StringTest, BytesToHex) {
  EXPECT_STREQ(BytesToHex(NULL, 0), _T(""));
  uint8 i = 0;
  EXPECT_STREQ(BytesToHex(&i, sizeof(i)), _T("00"));
  i = 0x7f;
  EXPECT_STREQ(BytesToHex(&i, sizeof(i)), _T("7f"));
  i = 0xff;
  EXPECT_STREQ(BytesToHex(&i, sizeof(i)), _T("ff"));

  // Assumes little-endian representation of integers.
  const uint32 array[] = {0x67452301, 0xefcdab89};
  EXPECT_STREQ(BytesToHex(reinterpret_cast<const uint8*>(array), sizeof(array)),
               _T("0123456789abcdef"));

  const uint8* first = reinterpret_cast<const uint8*>(array);
  const uint8* last  = first + sizeof(array);
  EXPECT_STREQ(BytesToHex(std::vector<uint8>(first, last)),
               _T("0123456789abcdef"));
}

TEST(StringTest, String_ToUpper) {
  // String_ToUpper is a wrapper over ::CharUpper.
  TCHAR s[] = _T("foo");
  String_ToUpper(s);
  EXPECT_STREQ(s, _T("FOO"));
}

TEST(StringTest, FormatResourceMessage_Valid) {
  EXPECT_STREQ(
      _T("Installation complete."),
      FormatResourceMessage(IDS_BUNDLE_INSTALLED_SUCCESSFULLY));

  EXPECT_STREQ(
      _T("The installer encountered error 12345: Action failed."),
      FormatResourceMessage(IDS_INSTALLER_FAILED_WITH_MESSAGE,
                            _T("12345"),
                            _T("Action failed.")));
}

TEST(StringTest, FormatResourceMessage_IdNotFound) {
  EXPECT_STREQ(_T(""), FormatResourceMessage(100000, "foo", 9));
}

TEST(StringTest, FormatErrorCode) {
  EXPECT_STREQ(_T("0xffffffff"), FormatErrorCode(static_cast<DWORD>(-1)));
  EXPECT_STREQ(_T("0"), FormatErrorCode(0));
  EXPECT_STREQ(_T("567"), FormatErrorCode(567));
  EXPECT_STREQ(_T("2147483647"), FormatErrorCode(0x7fffffff));
  EXPECT_STREQ(_T("0x80000000"), FormatErrorCode(0x80000000));
  EXPECT_STREQ(_T("0x80000001"), FormatErrorCode(0x80000001));
  EXPECT_STREQ(_T("0x8fffffff"), FormatErrorCode(0x8fffffff));
}

TEST(StringTest, Utf8BufferToWideChar) {
  // Unicode Greek capital letters.
  const TCHAR expected_string[] = {913, 914, 915, 916, 917, 918, 919, 920,
                                   921, 922, 923, 924, 925, 926, 927, 928,
                                   929, 931, 932, 933, 934, 935, 936, 937, 0};
  // Greek capital letters UTF-8 encoded.
  const char buffer[] = "ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ";
  CString actual_string = Utf8BufferToWideChar(
      std::vector<uint8>(buffer, buffer + arraysize(buffer)));
  EXPECT_STREQ(expected_string, actual_string);

  EXPECT_STREQ(_T(""), Utf8BufferToWideChar(std::vector<uint8>()));
}

TEST(StringTest, WideStringToUtf8UrlEncodedStringRoundTrip) {
  CString unicode_string;
  ASSERT_TRUE(unicode_string.LoadString(IDS_ESCAPE_TEST));

  // Convert from unicode to a wide representation of utf8,url encoded string.
  CString utf8encoded_str;
  ASSERT_HRESULT_SUCCEEDED(WideStringToUtf8UrlEncodedString(unicode_string,
                                                            &utf8encoded_str));
  ASSERT_FALSE(utf8encoded_str.IsEmpty());

  // Reconvert from the utf8, url encoded string to the wide version.
  CString out;
  ASSERT_HRESULT_SUCCEEDED(Utf8UrlEncodedStringToWideString(utf8encoded_str,
                                                            &out));
  ASSERT_FALSE(out.IsEmpty());
  ASSERT_STREQ(unicode_string, out);
}

TEST(StringTest, WideStringToUtf8UrlEncodedStringEmptyString) {
  CString unicode_string;

  // Convert from unicode to a wide representation of utf8,url encoded string.
  CString utf8encoded_str;
  ASSERT_HRESULT_SUCCEEDED(WideStringToUtf8UrlEncodedString(unicode_string,
                                                            &utf8encoded_str));
  ASSERT_TRUE(utf8encoded_str.IsEmpty());
}

TEST(StringTest, WideStringToUtf8UrlEncodedStringSpaces) {
  CString unicode_string(_T("   "));
  CStringA expected("%20%20%20");
  CString exp(expected);

  // Convert from unicode to a wide representation of utf8,url encoded string.
  CString utf8encoded_str;
  ASSERT_HRESULT_SUCCEEDED(WideStringToUtf8UrlEncodedString(unicode_string,
                                                            &utf8encoded_str));
  ASSERT_STREQ(exp, utf8encoded_str);
}

TEST(StringTest, WideStringToUtf8UrlEncodedStringTestString) {
  CString unicode_string(_T("Test Str/ing&values=&*^%$#"));
  CStringA ansi_exp("Test%20Str/ing%26values=%26*%5E%$#");
  CString exp(ansi_exp);

  // Convert from unicode to a wide representation of utf8,url encoded string.
  CString utf8encoded_str;
  ASSERT_HRESULT_SUCCEEDED(WideStringToUtf8UrlEncodedString(unicode_string,
                                                            &utf8encoded_str));
  ASSERT_STREQ(exp, utf8encoded_str);
}

TEST(StringTest, WideStringToUtf8UrlEncodedStringSimpleTestString) {
  CString unicode_string(_T("TestStr"));
  CStringA ansi_exp("TestStr");
  CString exp(ansi_exp);

  // Convert from unicode to a wide representation of utf8,url encoded string.
  CString utf8encoded_str;
  ASSERT_HRESULT_SUCCEEDED(WideStringToUtf8UrlEncodedString(unicode_string,
                                                            &utf8encoded_str));
  ASSERT_STREQ(exp, utf8encoded_str);
}

TEST(StringTest, Utf8UrlEncodedStringToWideStringEmpty) {
  // Convert from wide representation of utf8,url encoded string to unicode.
  CString unicode_string;
  CString utf8encoded_str;
  ASSERT_HRESULT_SUCCEEDED(Utf8UrlEncodedStringToWideString(utf8encoded_str,
                                                            &unicode_string));
  ASSERT_TRUE(unicode_string.IsEmpty());
}

TEST(StringTest, Utf8UrlEncodedStringToWideStringSpaces) {
  CString exp(_T("   "));
  CStringA utf8("%20%20%20");
  CString utf8encoded_str(utf8);

  // Convert from wide representation of utf8,url encoded string to unicode.
  CString unicode_string;
  ASSERT_HRESULT_SUCCEEDED(Utf8UrlEncodedStringToWideString(utf8encoded_str,
                                                            &unicode_string));
  ASSERT_STREQ(exp, unicode_string);
}

TEST(StringTest, Utf8UrlEncodedStringToWideStringSimpleString) {
  CString exp(_T("TestStr"));
  CStringA utf8("TestStr");
  CString utf8encoded_str(utf8);

  // Convert from wide representation of utf8,url encoded string to unicode.
  CString unicode_string;
  ASSERT_HRESULT_SUCCEEDED(Utf8UrlEncodedStringToWideString(utf8encoded_str,
                                                            &unicode_string));
  ASSERT_STREQ(exp, unicode_string);
}

TEST(StringTest, ConvertFileUriToLocalPath) {
  CString path_out;

  EXPECT_HRESULT_SUCCEEDED(ConvertFileUriToLocalPath(
    _T("file:///c:/test/path/one/test.txt"), &path_out));
  EXPECT_STREQ(_T("c:\\test\\path\\one\\test.txt"), path_out);

  EXPECT_HRESULT_SUCCEEDED(ConvertFileUriToLocalPath(
    _T("file:///d:/path%20with%20spaces/test.txt"), &path_out));
  EXPECT_STREQ(_T("d:\\path with spaces\\test.txt"), path_out);

  EXPECT_HRESULT_SUCCEEDED(ConvertFileUriToLocalPath(
    _T("file://e|/pipe_for_colon/test.txt"), &path_out));
  EXPECT_STREQ(_T("e:\\pipe_for_colon\\test.txt"), path_out);

  // The unescaped & is illegal in both URIs and Win32 paths, and the escaped
  // bracket is illegal in a Win32 path.  However, we should pass them through
  // and allow the file system layer to reject it at an appropriate spot.
  EXPECT_HRESULT_SUCCEEDED(ConvertFileUriToLocalPath(
    _T("file://f:/path_with_illegal_characters/te&st%60.txt"), &path_out));
  EXPECT_STREQ(_T("f:\\path_with_illegal_characters\\te&st%60.txt"), path_out);

  EXPECT_HRESULT_SUCCEEDED(ConvertFileUriToLocalPath(
    _T("file://g:/url_has_get_arguments/test.txt?q=abcde"), &path_out));
  EXPECT_STREQ(_T("g:\\url_has_get_arguments\\test.txt"), path_out);

  EXPECT_HRESULT_FAILED(ConvertFileUriToLocalPath(
    _T("i:\\actual_path_instead_of_url\\test.txt"), &path_out));
  EXPECT_HRESULT_FAILED(ConvertFileUriToLocalPath(
    _T("http://uri_uses_another_scheme.com/test.txt"), &path_out));
  EXPECT_HRESULT_FAILED(ConvertFileUriToLocalPath(_T("not_a_uri"), &path_out));
  EXPECT_HRESULT_FAILED(ConvertFileUriToLocalPath(_T(""), &path_out));
}

TEST(HexAndBack, a2b_and_b2a) {
  string upper_and_lower = "0123456789abcdefABCDEF";
  string upper_and_lower_raw = "\x01\x23\x45\x67\x89\xab\xcd\xef\xAB\xCD\xEF";
  string upper_and_lower_RT = "0123456789abcdefabcdef";

  string upper_and_lower_a2b = a2b_hex(upper_and_lower);
  EXPECT_EQ(upper_and_lower_raw, upper_and_lower_a2b);

  string upper_and_lower_raw_b2a = b2a_hex(upper_and_lower_raw.data(),
                                                upper_and_lower_raw.size());
  EXPECT_EQ(upper_and_lower_RT, upper_and_lower_raw_b2a);
}

TEST(StringTest, SafeHexStringToVector) {
  std::vector<uint8> out;

  EXPECT_FALSE(SafeHexStringToVector(_T(""), &out));
  EXPECT_FALSE(SafeHexStringToVector(_T("a"), &out));
  EXPECT_TRUE( SafeHexStringToVector(_T("ab"), &out));
  EXPECT_FALSE(SafeHexStringToVector(_T("a!"), &out));
  EXPECT_FALSE(SafeHexStringToVector(_T("abc"), &out));
  EXPECT_FALSE(SafeHexStringToVector(_T("ab!"), &out));
  EXPECT_TRUE( SafeHexStringToVector(_T("0123456789abcdef"), &out));

  EXPECT_FALSE(SafeHexStringToVector("", &out));
  EXPECT_FALSE(SafeHexStringToVector("a", &out));
  EXPECT_TRUE( SafeHexStringToVector("ab", &out));
  EXPECT_FALSE(SafeHexStringToVector("a!", &out));
  EXPECT_FALSE(SafeHexStringToVector("abc", &out));
  EXPECT_FALSE(SafeHexStringToVector("ab!", &out));
  EXPECT_TRUE( SafeHexStringToVector("0123456789abcdef", &out));
}

TEST(StringTest, WideToUtf8Vector) {
  std::vector<uint8> out;

  CStringW empty = _T("");
  WideToUtf8Vector(empty, &out);
  EXPECT_EQ(0, out.size());

  CStringW ansi_only = _T("abc");
  WideToUtf8Vector(ansi_only, &out);
  EXPECT_EQ(3, out.size());

  // U+266B = Beamed Eighth Notes (should encode to 3 bytes)
  // U+0416 = Cyrillic Capital Letter Zhe (should encode to 2 bytes)
  // Those two characters, plus three ASCII chars, should produce 8 bytes.

#pragma warning(push)
  // warning C4428: universal-character-name encountered in source
#pragma warning(disable:4428)
  CStringW multibytes = _T("_\u266B_\u0416_");
#pragma warning(pop)

  WideToUtf8Vector(multibytes, &out);
  EXPECT_EQ(8, out.size());
}

}  // namespace omaha

