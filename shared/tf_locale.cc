#include <tuple>
#include <locale.h>
#include <string>
#include <vector>
#include <string.h>
#include <iostream>

#include "tf_locale.hh"

namespace tgf {

enum Lang HOST_LANG = Lang::unknown;
enum Lang PREFER_LANG = Lang::unknown;

//
// Currently supported languages: en_HK, en_US, zh_CN, zh_HK
//
const char *STRNAME_unknown = "?-?";
// const char *STRNAME_aa_DJ = "aa-DJ";
// const char *STRNAME_af_ZA = "af-ZA";
// const char *STRNAME_an_ES = "an-ES";
// const char *STRNAME_ar_AE = "ar-AE";
// const char *STRNAME_ar_BH = "ar-BH";
// const char *STRNAME_ar_DZ = "ar-DZ";
// const char *STRNAME_ar_EG = "ar-EG";
// const char *STRNAME_ar_IQ = "ar-IQ";
// const char *STRNAME_ar_JO = "ar-JO";
// const char *STRNAME_ar_KW = "ar-KW";
// const char *STRNAME_ar_LB = "ar-LB";
// const char *STRNAME_ar_LY = "ar-LY";
// const char *STRNAME_ar_MA = "ar-MA";
// const char *STRNAME_ar_OM = "ar-OM";
// const char *STRNAME_ar_QA = "ar-QA";
// const char *STRNAME_ar_SA = "ar-SA";
// const char *STRNAME_ar_SD = "ar-SD";
// const char *STRNAME_ar_SY = "ar-SY";
// const char *STRNAME_ar_TN = "ar-TN";
// const char *STRNAME_ar_YE = "ar-YE";
// const char *STRNAME_ast_ES = "ast-ES";
// const char *STRNAME_be_BY = "be-BY";
// const char *STRNAME_bg_BG = "bg-BG";
// const char *STRNAME_bhb_IN = "bhb-IN";
// const char *STRNAME_br_FR = "br-FR";
// const char *STRNAME_bs_BA = "bs-BA";
// const char *STRNAME_ca_AD = "ca-AD";
// const char *STRNAME_ca_ES = "ca-ES";
// const char *STRNAME_ca_FR = "ca-FR";
// const char *STRNAME_ca_IT = "ca-IT";
// const char *STRNAME_cs_CZ = "cs-CZ";
// const char *STRNAME_cy_GB = "cy-GB";
// const char *STRNAME_da_DK = "da-DK";
// const char *STRNAME_de_AT = "de-AT";
// const char *STRNAME_de_BE = "de-BE";
// const char *STRNAME_de_CH = "de-CH";
// const char *STRNAME_de_DE = "de-DE";
// const char *STRNAME_de_IT = "de-IT";
// const char *STRNAME_de_LI = "de-LI";
// const char *STRNAME_de_LU = "de-LU";
// const char *STRNAME_el_CY = "el-CY";
// const char *STRNAME_el_GR = "el-GR";
// const char *STRNAME_en_AU = "en-AU";
// const char *STRNAME_en_BW = "en-BW";
// const char *STRNAME_en_CA = "en-CA";
// const char *STRNAME_en_DK = "en-DK";
// const char *STRNAME_en_GB = "en-GB";
const char *STRNAME_en_HK = "en-HK";
// const char *STRNAME_en_IE = "en-IE";
// const char *STRNAME_en_NZ = "en-NZ";
// const char *STRNAME_en_PH = "en-PH";
// const char *STRNAME_en_SC = "en-SC";
// const char *STRNAME_en_SG = "en-SG";
const char *STRNAME_en_US = "en-US";
// const char *STRNAME_en_ZA = "en-ZA";
const char *STRNAME_en_ZW = "en-ZW";
// const char *STRNAME_es_AR = "es-AR";
// const char *STRNAME_es_BO = "es-BO";
// const char *STRNAME_es_CL = "es-CL";
// const char *STRNAME_es_CO = "es-CO";
// const char *STRNAME_es_CR = "es-CR";
// const char *STRNAME_es_DO = "es-DO";
// const char *STRNAME_es_EC = "es-EC";
// const char *STRNAME_es_ES = "es-ES";
// const char *STRNAME_es_GT = "es-GT";
// const char *STRNAME_es_HN = "es-HN";
// const char *STRNAME_es_MX = "es-MX";
// const char *STRNAME_es_NI = "es-NI";
// const char *STRNAME_es_PA = "es-PA";
// const char *STRNAME_es_PE = "es-PE";
// const char *STRNAME_es_PR = "es-PR";
// const char *STRNAME_es_PY = "es-PY";
// const char *STRNAME_es_SV = "es-SV";
// const char *STRNAME_es_US = "es-US";
// const char *STRNAME_es_UY = "es-UY";
// const char *STRNAME_es_VE = "es-VE";
// const char *STRNAME_et_EE = "et-EE";
// const char *STRNAME_eu_ES = "eu-ES";
// const char *STRNAME_eu_FR = "eu-FR";
// const char *STRNAME_fi_FI = "fi-FI";
// const char *STRNAME_fo_FO = "fo-FO";
// const char *STRNAME_fr_BE = "fr-BE";
// const char *STRNAME_fr_CA = "fr-CA";
// const char *STRNAME_fr_CH = "fr-CH";
// const char *STRNAME_fr_FR = "fr-FR";
// const char *STRNAME_fr_LU = "fr-LU";
// const char *STRNAME_ga_IE = "ga-IE";
// const char *STRNAME_gd_GB = "gd-GB";
// const char *STRNAME_gl_ES = "gl-ES";
// const char *STRNAME_gv_GB = "gv-GB";
// const char *STRNAME_he_IL = "he-IL";
// const char *STRNAME_hr_HR = "hr-HR";
// const char *STRNAME_hsb_DE = "hsb-DE";
// const char *STRNAME_hu_HU = "hu-HU";
// const char *STRNAME_id_ID = "id-ID";
// const char *STRNAME_is_IS = "is-IS";
// const char *STRNAME_it_CH = "it-CH";
// const char *STRNAME_it_IT = "it-IT";
// const char *STRNAME_ja_JP = "ja-JP";
// const char *STRNAME_ka_GE = "ka-GE";
// const char *STRNAME_kk_KZ = "kk-KZ";
// const char *STRNAME_kl_GL = "kl-GL";
// const char *STRNAME_ko_KR = "ko-KR";
// const char *STRNAME_ku_TR = "ku-TR";
// const char *STRNAME_kw_GB = "kw-GB";
// const char *STRNAME_lg_UG = "lg-UG";
// const char *STRNAME_lt_LT = "lt-LT";
// const char *STRNAME_lv_LV = "lv-LV";
// const char *STRNAME_mg_MG = "mg-MG";
// const char *STRNAME_mi_NZ = "mi-NZ";
// const char *STRNAME_mk_MK = "mk-MK";
// const char *STRNAME_ms_MY = "ms-MY";
// const char *STRNAME_mt_MT = "mt-MT";
// const char *STRNAME_nb_NO = "nb-NO";
// const char *STRNAME_nl_BE = "nl-BE";
// const char *STRNAME_nl_NL = "nl-NL";
// const char *STRNAME_nn_NO = "nn-NO";
// const char *STRNAME_oc_FR = "oc-FR";
// const char *STRNAME_om_KE = "om-KE";
// const char *STRNAME_pl_PL = "pl-PL";
// const char *STRNAME_pt_BR = "pt-BR";
// const char *STRNAME_pt_PT = "pt-PT";
// const char *STRNAME_ro_RO = "ro-RO";
// const char *STRNAME_ru_RU = "ru-RU";
// const char *STRNAME_ru_UA = "ru-UA";
// const char *STRNAME_sk_SK = "sk-SK";
// const char *STRNAME_sl_SI = "sl-SI";
// const char *STRNAME_so_DJ = "so-DJ";
// const char *STRNAME_so_KE = "so-KE";
// const char *STRNAME_so_SO = "so-SO";
// const char *STRNAME_sq_AL = "sq-AL";
// const char *STRNAME_st_ZA = "st-ZA";
// const char *STRNAME_sv_FI = "sv-FI";
// const char *STRNAME_sv_SE = "sv-SE";
// const char *STRNAME_tcy_IN = "tcy-IN";
// const char *STRNAME_tg_TJ = "tg-TJ";
// const char *STRNAME_th_TH = "th-TH";
// const char *STRNAME_tl_PH = "tl-PH";
// const char *STRNAME_tr_CY = "tr-CY";
// const char *STRNAME_tr_TR = "tr-TR";
// const char *STRNAME_uk_UA = "uk-UA";
// const char *STRNAME_uz_UZ = "uz-UZ";
// const char *STRNAME_wa_BE = "wa-BE";
// const char *STRNAME_xh_ZA = "xh-ZA";
// const char *STRNAME_yi_US = "yi-US";
const char *STRNAME_zh_CN = "zh-CN";
const char *STRNAME_zh_HK = "zh-HK";
// const char *STRNAME_zh_SG = "zh-SG";
// const char *STRNAME_zh_TW = "zh-TW";
// const char *STRNAME_zu_ZA = "zu-ZA";

extern Lang
lang_from_cstr (const char *cstr)
{
  if (strlen (cstr) < 5)
    return Lang::unknown;

  // if (strcmp (cstr, STRNAME_aa_DJ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_af_ZA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_an_ES) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_AE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_BH) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_DZ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_EG) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_IQ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_JO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_KW) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_LB) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_LY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_MA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_OM) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_QA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_SA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_SD) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_SY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_TN) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ar_YE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ast_ES) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_be_BY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_bg_BG) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_bhb_IN) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_br_FR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_bs_BA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ca_AD) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ca_ES) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ca_FR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ca_IT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_cs_CZ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_cy_GB) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_da_DK) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_AT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_BE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_CH) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_DE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_IT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_LI) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_de_LU) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_el_CY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_el_GR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_en_AU) == 0)
  //   return Lang::en_AU;
  // if (strcmp (cstr, STRNAME_en_BW) == 0)
  //   return Lang::en_BW;
  // if (strcmp (cstr, STRNAME_en_CA) == 0)
  //   return Lang::en_CA;
  // if (strcmp (cstr, STRNAME_en_DK) == 0)
  //   return Lang::en_DK;
  // if (strcmp (cstr, STRNAME_en_GB) == 0)
  //   return Lang::en_GB;
  if (strcmp (cstr, STRNAME_en_HK) == 0)
    return Lang::en_HK;
  // if (strcmp (cstr, STRNAME_en_IE) == 0)
  //   return Lang::en_IE;
  // if (strcmp (cstr, STRNAME_en_NZ) == 0)
  //   return Lang::en_NZ;
  // if (strcmp (cstr, STRNAME_en_PH) == 0)
  //   return Lang::en_PH;
  // if (strcmp (cstr, STRNAME_en_SC) == 0)
  //   return Lang::en_SC;
  // if (strcmp (cstr, STRNAME_en_SG) == 0)
  //   return Lang::en_SG;
  if (strcmp (cstr, STRNAME_en_US) == 0)
    return Lang::en_US;
  // if (strcmp (cstr, STRNAME_en_ZA) == 0)
  //   return Lang::en_ZA;
  if (strcmp (cstr, STRNAME_en_ZW) == 0)
    return Lang::en_ZW;
  // if (strcmp (cstr, STRNAME_es_AR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_BO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_CL) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_CO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_CR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_DO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_EC) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_ES) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_GT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_HN) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_MX) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_NI) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_PA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_PE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_PR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_PY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_SV) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_US) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_UY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_es_VE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_et_EE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_eu_ES) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_eu_FR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fi_FI) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fo_FO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fr_BE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fr_CA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fr_CH) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fr_FR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_fr_LU) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ga_IE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_gd_GB) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_gl_ES) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_gv_GB) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_he_IL) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_hr_HR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_hsb_DE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_hu_HU) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_id_ID) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_is_IS) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_it_CH) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_it_IT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ja_JP) == 0)
  //   return Lang::ja_JP;
  // if (strcmp (cstr, STRNAME_ka_GE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_kk_KZ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_kl_GL) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ko_KR) == 0)
  //   return Lang::ko_KR;
  // if (strcmp (cstr, STRNAME_ku_TR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_kw_GB) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_lg_UG) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_lt_LT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_lv_LV) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_mg_MG) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_mi_NZ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_mk_MK) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ms_MY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_mt_MT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_nb_NO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_nl_BE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_nl_NL) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_nn_NO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_oc_FR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_om_KE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_pl_PL) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_pt_BR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_pt_PT) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ro_RO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ru_RU) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_ru_UA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_sk_SK) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_sl_SI) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_so_DJ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_so_KE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_so_SO) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_sq_AL) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_st_ZA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_sv_FI) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_sv_SE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_tcy_IN) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_tg_TJ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_th_TH) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_tl_PH) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_tr_CY) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_tr_TR) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_uk_UA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_uz_UZ) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_wa_BE) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_xh_ZA) == 0)
  //   return Lang::unknown;
  // if (strcmp (cstr, STRNAME_yi_US) == 0)
  //   return Lang::unknown;
  if (strcmp (cstr, STRNAME_zh_CN) == 0)
    return Lang::zh_CN;
  if (strcmp (cstr, STRNAME_zh_HK) == 0)
    return Lang::zh_HK;
  // if (strcmp (cstr, STRNAME_zh_SG) == 0)
  //   return Lang::zh_SG;
  // if (strcmp (cstr, STRNAME_zh_TW) == 0)
  //   return Lang::zh_TW;
  // if (strcmp (cstr, STRNAME_zu_ZA) == 0)
  //   return Lang::unknown;

  return Lang::unknown;
}

const char *
lang_to_cstr (Lang l)
{
  switch (l)
    {
    // case Lang::aa_DJ:
    //   return STRNAME_aa_DJ;
    //   break;
    // case Lang::af_ZA:
    //   return STRNAME_af_ZA;
    //   break;
    // case Lang::an_ES:
    //   return STRNAME_an_ES;
    //   break;
    // case Lang::ar_AE:
    //   return STRNAME_ar_AE;
    //   break;
    // case Lang::ar_BH:
    //   return STRNAME_ar_BH;
    //   break;
    // case Lang::ar_DZ:
    //   return STRNAME_ar_DZ;
    //   break;
    // case Lang::ar_EG:
    //   return STRNAME_ar_EG;
    //   break;
    // case Lang::ar_IQ:
    //   return STRNAME_ar_IQ;
    //   break;
    // case Lang::ar_JO:
    //   return STRNAME_ar_JO;
    //   break;
    // case Lang::ar_KW:
    //   return STRNAME_ar_KW;
    //   break;
    // case Lang::ar_LB:
    //   return STRNAME_ar_LB;
    //   break;
    // case Lang::ar_LY:
    //   return STRNAME_ar_LY;
    //   break;
    // case Lang::ar_MA:
    //   return STRNAME_ar_MA;
    //   break;
    // case Lang::ar_OM:
    //   return STRNAME_ar_OM;
    //   break;
    // case Lang::ar_QA:
    //   return STRNAME_ar_QA;
    //   break;
    // case Lang::ar_SA:
    //   return STRNAME_ar_SA;
    //   break;
    // case Lang::ar_SD:
    //   return STRNAME_ar_SD;
    //   break;
    // case Lang::ar_SY:
    //   return STRNAME_ar_SY;
    //   break;
    // case Lang::ar_TN:
    //   return STRNAME_ar_TN;
    //   break;
    // case Lang::ar_YE:
    //   return STRNAME_ar_YE;
    //   break;
    // case Lang::ast_ES:
    //   return STRNAME_ast_ES;
    //   break;
    // case Lang::be_BY:
    //   return STRNAME_be_BY;
    //   break;
    // case Lang::bg_BG:
    //   return STRNAME_bg_BG;
    //   break;
    // case Lang::bhb_IN:
    //   return STRNAME_bhb_IN;
    //   break;
    // case Lang::br_FR:
    //   return STRNAME_br_FR;
    //   break;
    // case Lang::bs_BA:
    //   return STRNAME_bs_BA;
    //   break;
    // case Lang::ca_AD:
    //   return STRNAME_ca_AD;
    //   break;
    // case Lang::ca_ES:
    //   return STRNAME_ca_ES;
    //   break;
    // case Lang::ca_FR:
    //   return STRNAME_ca_FR;
    //   break;
    // case Lang::ca_IT:
    //   return STRNAME_ca_IT;
    //   break;
    // case Lang::cs_CZ:
    //   return STRNAME_cs_CZ;
    //   break;
    // case Lang::cy_GB:
    //   return STRNAME_cy_GB;
    //   break;
    // case Lang::da_DK:
    //   return STRNAME_da_DK;
    //   break;
    // case Lang::de_AT:
    //   return STRNAME_de_AT;
    //   break;
    // case Lang::de_BE:
    //   return STRNAME_de_BE;
    //   break;
    // case Lang::de_CH:
    //   return STRNAME_de_CH;
    //   break;
    // case Lang::de_DE:
    //   return STRNAME_de_DE;
    //   break;
    // case Lang::de_IT:
    //   return STRNAME_de_IT;
    //   break;
    // case Lang::de_LI:
    //   return STRNAME_de_LI;
    //   break;
    // case Lang::de_LU:
    //   return STRNAME_de_LU;
    //   break;
    // case Lang::el_CY:
    //   return STRNAME_el_CY;
    //   break;
    // case Lang::el_GR:
    //   return STRNAME_el_GR;
    //   break;
    // case Lang::en_AU:
    //   return STRNAME_en_AU;
    //   break;
    // case Lang::en_BW:
    //   return STRNAME_en_BW;
    //   break;
    // case Lang::en_CA:
    //   return STRNAME_en_CA;
    //   break;
    // case Lang::en_DK:
    //   return STRNAME_en_DK;
    //   break;
    // case Lang::en_GB:
    //   return STRNAME_en_GB;
    //   break;
    case Lang::en_HK:
      return STRNAME_en_HK;
      break;
    // case Lang::en_IE:
    //   return STRNAME_en_IE;
    //   break;
    // case Lang::en_NZ:
    //   return STRNAME_en_NZ;
    //   break;
    // case Lang::en_PH:
    //   return STRNAME_en_PH;
    //   break;
    // case Lang::en_SC:
    //   return STRNAME_en_SC;
    //   break;
    // case Lang::en_SG:
    //   return STRNAME_en_SG;
    //   break;
    case Lang::en_US:
      return STRNAME_en_US;
      break;
    // case Lang::en_ZA:
    //   return STRNAME_en_ZA;
    //   break;
    case Lang::en_ZW:
      return STRNAME_en_ZW;
    //   break;
    // case Lang::es_AR:
    //   return STRNAME_es_AR;
    //   break;
    // case Lang::es_BO:
    //   return STRNAME_es_BO;
    //   break;
    // case Lang::es_CL:
    //   return STRNAME_es_CL;
    //   break;
    // case Lang::es_CO:
    //   return STRNAME_es_CO;
    //   break;
    // case Lang::es_CR:
    //   return STRNAME_es_CR;
    //   break;
    // case Lang::es_DO:
    //   return STRNAME_es_DO;
    //   break;
    // case Lang::es_EC:
    //   return STRNAME_es_EC;
    //   break;
    // case Lang::es_ES:
    //   return STRNAME_es_ES;
    //   break;
    // case Lang::es_GT:
    //   return STRNAME_es_GT;
    //   break;
    // case Lang::es_HN:
    //   return STRNAME_es_HN;
    //   break;
    // case Lang::es_MX:
    //   return STRNAME_es_MX;
    //   break;
    // case Lang::es_NI:
    //   return STRNAME_es_NI;
    //   break;
    // case Lang::es_PA:
    //   return STRNAME_es_PA;
    //   break;
    // case Lang::es_PE:
    //   return STRNAME_es_PE;
    //   break;
    // case Lang::es_PR:
    //   return STRNAME_es_PE;
    //   break;
    // case Lang::es_PY:
    //   return STRNAME_es_PY;
    //   break;
    // case Lang::es_SV:
    //   return STRNAME_es_SV;
    //   break;
    // case Lang::es_US:
    //   return STRNAME_es_US;
    //   break;
    // case Lang::es_UY:
    //   return STRNAME_es_UY;
    //   break;
    // case Lang::es_VE:
    //   return STRNAME_es_VE;
    //   break;
    // case Lang::et_EE:
    //   return STRNAME_et_EE;
    //   break;
    // case Lang::eu_ES:
    //   return STRNAME_eu_ES;
    //   break;
    // case Lang::eu_FR:
    //   return STRNAME_eu_FR;
    //   break;
    // case Lang::fi_FI:
    //   return STRNAME_fi_FI;
    //   break;
    // case Lang::fo_FO:
    //   return STRNAME_fo_FO;
    //   break;
    // case Lang::fr_BE:
    //   return STRNAME_fr_BE;
    //   break;
    // case Lang::fr_CA:
    //   return STRNAME_fr_CA;
    //   break;
    // case Lang::fr_CH:
    //   return STRNAME_fr_CH;
    //   break;
    // case Lang::fr_FR:
    //   return STRNAME_fr_FR;
    //   break;
    // case Lang::fr_LU:
    //   return STRNAME_fr_LU;
    //   break;
    // case Lang::ga_IE:
    //   return STRNAME_ga_IE;
    //   break;
    // case Lang::gd_GB:
    //   return STRNAME_gd_GB;
    //   break;
    // case Lang::gl_ES:
    //   return STRNAME_gl_ES;
    //   break;
    // case Lang::gv_GB:
    //   return STRNAME_gv_GB;
    //   break;
    // case Lang::he_IL:
    //   return STRNAME_he_IL;
    //   break;
    // case Lang::hr_HR:
    //   return STRNAME_hr_HR;
    //   break;
    // case Lang::hsb_DE:
    //   return STRNAME_hsb_DE;
    //   break;
    // case Lang::hu_HU:
    //   return STRNAME_hu_HU;
    //   break;
    // case Lang::id_ID:
    //   return STRNAME_id_ID;
    //   break;
    // case Lang::is_IS:
    //   return STRNAME_is_IS;
    //   break;
    // case Lang::it_CH:
    //   return STRNAME_it_CH;
    //   break;
    // case Lang::it_IT:
    //   return STRNAME_it_IT;
    //   break;
    // case Lang::ja_JP:
    //   return STRNAME_ja_JP;
    //   break;
    // case Lang::ka_GE:
    //   return STRNAME_ka_GE;
    //   break;
    // case Lang::kk_KZ:
    //   return STRNAME_kk_KZ;
    //   break;
    // case Lang::kl_GL:
    //   return STRNAME_kl_GL;
    //   break;
    // case Lang::ko_KR:
    //   return STRNAME_ko_KR;
    //   break;
    // case Lang::ku_TR:
    //   return STRNAME_ku_TR;
    //   break;
    // case Lang::kw_GB:
    //   return STRNAME_kw_GB;
    //   break;
    // case Lang::lg_UG:
    //   return STRNAME_lg_UG;
    //   break;
    // case Lang::lt_LT:
    //   return STRNAME_lt_LT;
    //   break;
    // case Lang::lv_LV:
    //   return STRNAME_lv_LV;
    //   break;
    // case Lang::mg_MG:
    //   return STRNAME_mg_MG;
    //   break;
    // case Lang::mi_NZ:
    //   return STRNAME_mi_NZ;
    //   break;
    // case Lang::mk_MK:
    //   return STRNAME_mk_MK;
    //   break;
    // case Lang::ms_MY:
    //   return STRNAME_ms_MY;
    //   break;
    // case Lang::mt_MT:
    //   return STRNAME_mt_MT;
    //   break;
    // case Lang::nb_NO:
    //   return STRNAME_nb_NO;
    //   break;
    // case Lang::nl_BE:
    //   return STRNAME_nl_NL;
    //   break;
    // case Lang::nl_NL:
    //   return STRNAME_nl_NL;
    //   break;
    // case Lang::nn_NO:
    //   return STRNAME_nn_NO;
    //   break;
    // case Lang::oc_FR:
    //   return STRNAME_oc_FR;
    //   break;
    // case Lang::om_KE:
    //   return STRNAME_om_KE;
    //   break;
    // case Lang::pl_PL:
    //   return STRNAME_pl_PL;
    //   break;
    // case Lang::pt_BR:
    //   return STRNAME_pt_BR;
    //   break;
    // case Lang::pt_PT:
    //   return STRNAME_pt_PT;
    //   break;
    // case Lang::ro_RO:
    //   return STRNAME_ro_RO;
    //   break;
    // case Lang::ru_RU:
    //   return STRNAME_ru_RU;
    //   break;
    // case Lang::ru_UA:
    //   return STRNAME_ru_UA;
    //   break;
    // case Lang::sk_SK:
    //   return STRNAME_sk_SK;
    //   break;
    // case Lang::sl_SI:
    //   return STRNAME_sl_SI;
    //   break;
    // case Lang::so_DJ:
    //   return STRNAME_so_DJ;
    //   break;
    // case Lang::so_KE:
    //   return STRNAME_so_KE;
    //   break;
    // case Lang::so_SO:
    //   return STRNAME_so_SO;
    //   break;
    // case Lang::sq_AL:
    //   return STRNAME_sq_AL;
    //   break;
    // case Lang::st_ZA:
    //   return STRNAME_st_ZA;
    //   break;
    // case Lang::sv_FI:
    //   return STRNAME_sv_FI;
    //   break;
    // case Lang::sv_SE:
    //   return STRNAME_sv_SE;
    //   break;
    // case Lang::tcy_IN:
    //   return STRNAME_tcy_IN;
    //   break;
    // case Lang::tg_TJ:
    //   return STRNAME_tg_TJ;
    //   break;
    // case Lang::th_TH:
    //   return STRNAME_th_TH;
    //   break;
    // case Lang::tl_PH:
    //   return STRNAME_tl_PH;
    //   break;
    // case Lang::tr_CY:
    //   return STRNAME_tr_CY;
    //   break;
    // case Lang::tr_TR:
    //   return STRNAME_tr_TR;
    //   break;
    // case Lang::uk_UA:
    //   return STRNAME_uk_UA;
    //   break;
    // case Lang::uz_UZ:
    //   return STRNAME_uz_UZ;
    //   break;
    // case Lang::wa_BE:
    //   return STRNAME_wa_BE;
    //   break;
    // case Lang::xh_ZA:
    //   return STRNAME_xh_ZA;
    //   break;
    // case Lang::yi_US:
    //   return STRNAME_yi_US;
    //   break;
    case Lang::zh_CN:
      return STRNAME_zh_CN;
      break;
    case Lang::zh_HK:
      return STRNAME_zh_HK;
      break;
    // case Lang::zh_SG:
    //   return STRNAME_zh_SG;
    //   break;
    // case Lang::zh_TW:
    //   return STRNAME_zh_TW;
    //   break;
    // case Lang::zu_ZA:
    //   return STRNAME_zu_ZA;
    //   break;
    default:
      return STRNAME_unknown;
      break;
    }
}

std::ostream &
operator<< (std::ostream &os, const Lang l)
{
  os << lang_to_cstr (l);
  return os;
}

bool
try_ensure_locale ()
{
  using namespace std;

  // FIXME: use constant instead
  vector<tuple<const char *, Lang>> lclist = {
    // {"C.UTF-8", unknown}, // we dont need this
    // {STRNAME_aa_DJ, aa_DJ}, {STRNAME_af_ZA, af_ZA}, {STRNAME_an_ES, an_ES},
    // {STRNAME_ar_AE, ar_AE}, {STRNAME_ar_BH, ar_BH}, {STRNAME_ar_DZ, ar_DZ},
    // {STRNAME_ar_EG, ar_EG}, {STRNAME_ar_IQ, ar_IQ}, {STRNAME_ar_JO, ar_JO},
    // {STRNAME_ar_KW, ar_KW}, {STRNAME_ar_LB, ar_LB}, {STRNAME_ar_LY, ar_LY},
    // {STRNAME_ar_MA, ar_MA}, {STRNAME_ar_OM, ar_OM}, {STRNAME_ar_QA, ar_QA},
    // {STRNAME_ar_SA, ar_SA}, {STRNAME_ar_SD, ar_SD}, {STRNAME_ar_SY, ar_SY},
    // {STRNAME_ar_TN, ar_TN}, {STRNAME_ar_YE, ar_YE}, {STRNAME_ast_ES, ast_ES},
    // {STRNAME_be_BY, be_BY}, {STRNAME_bg_BG, bg_BG}, {STRNAME_bhb_IN, bhb_IN},
    // {STRNAME_br_FR, br_FR}, {STRNAME_bs_BA, bs_BA}, {STRNAME_ca_AD, ca_AD},
    // {STRNAME_ca_ES, ca_ES}, {STRNAME_ca_FR, ca_FR}, {STRNAME_ca_IT, ca_IT},
    // {STRNAME_cs_CZ, cs_CZ}, {STRNAME_cy_GB, cy_GB}, {STRNAME_da_DK, da_DK},
    // {STRNAME_de_AT, de_AT}, {STRNAME_de_BE, de_BE}, {STRNAME_de_CH, de_CH},
    // {STRNAME_de_DE, de_DE}, {STRNAME_de_IT, de_IT}, {STRNAME_de_LI, de_LI},
    // {STRNAME_de_LU, de_LU}, {STRNAME_el_CY, el_CY}, {STRNAME_el_GR, el_GR},
    // {STRNAME_en_AU, en_AU}, {STRNAME_en_BW, en_BW}, {STRNAME_en_CA, en_CA},
    // {STRNAME_en_DK, en_DK}, {STRNAME_en_GB, en_GB},
    {STRNAME_en_HK, en_HK},
    // {STRNAME_en_IE, en_IE}, {STRNAME_en_NZ, en_NZ}, {STRNAME_en_PH, en_PH},
    // {STRNAME_en_SC, en_SC}, {STRNAME_en_SG, en_SG},
    {STRNAME_en_US, en_US},
    // {STRNAME_en_ZA, en_ZA},
    {STRNAME_en_ZW, en_ZW},
    // {STRNAME_es_AR, es_AR},
    // {STRNAME_es_BO, es_BO}, {STRNAME_es_CL, es_CL}, {STRNAME_es_CO, es_CO},
    // {STRNAME_es_CR, es_CR}, {STRNAME_es_DO, es_DO}, {STRNAME_es_EC, es_EC},
    // {STRNAME_es_ES, es_ES}, {STRNAME_es_GT, es_GT}, {STRNAME_es_HN, es_HN},
    // {STRNAME_es_MX, es_MX}, {STRNAME_es_NI, es_NI}, {STRNAME_es_PA, es_PA},
    // {STRNAME_es_PE, es_PE}, {STRNAME_es_PR, es_PR}, {STRNAME_es_PY, es_PY},
    // {STRNAME_es_SV, es_SV}, {STRNAME_es_US, es_US}, {STRNAME_es_UY, es_UY},
    // {STRNAME_es_VE, es_VE}, {STRNAME_et_EE, et_EE}, {STRNAME_eu_ES, eu_ES},
    // {STRNAME_eu_FR, eu_FR}, {STRNAME_fi_FI, fi_FI}, {STRNAME_fo_FO, eu_FR},
    // {STRNAME_fr_BE, fr_BE}, {STRNAME_fr_CA, fr_CA}, {STRNAME_fr_CH, fr_CH},
    // {STRNAME_fr_FR, fr_FR}, {STRNAME_fr_LU, fr_LU}, {STRNAME_ga_IE, ga_IE},
    // {STRNAME_gd_GB, gd_GB}, {STRNAME_gl_ES, gl_ES}, {STRNAME_gv_GB, gv_GB},
    // {STRNAME_he_IL, he_IL}, {STRNAME_hr_HR, hr_HR}, {STRNAME_hsb_DE, hsb_DE},
    // {STRNAME_hu_HU, hu_HU}, {STRNAME_id_ID, id_ID}, {STRNAME_is_IS, is_IS},
    // {STRNAME_it_CH, it_CH}, {STRNAME_it_IT, it_IT}, {STRNAME_ja_JP, ja_JP},
    // {STRNAME_ka_GE, ka_GE}, {STRNAME_kk_KZ, kk_KZ}, {STRNAME_kl_GL, kl_GL},
    // {STRNAME_ko_KR, ko_KR}, {STRNAME_ku_TR, ku_TR}, {STRNAME_kw_GB, kw_GB},
    // {STRNAME_lg_UG, lg_UG}, {STRNAME_lt_LT, lt_LT}, {STRNAME_lv_LV, lv_LV},
    // {STRNAME_mg_MG, mg_MG}, {STRNAME_mi_NZ, mi_NZ}, {STRNAME_mk_MK, mk_MK},
    // {STRNAME_ms_MY, ms_MY}, {STRNAME_mt_MT, mt_MT}, {STRNAME_nb_NO, nb_NO},
    // {STRNAME_nl_BE, nl_BE}, {STRNAME_nl_NL, nl_NL}, {STRNAME_nn_NO, nn_NO},
    // {STRNAME_oc_FR, oc_FR}, {STRNAME_om_KE, om_KE}, {STRNAME_pl_PL, pl_PL},
    // {STRNAME_pt_BR, pt_BR}, {STRNAME_pt_PT, pt_PT}, {STRNAME_ro_RO, ro_RO},
    // {STRNAME_ru_RU, ru_RU}, {STRNAME_ru_UA, ru_UA}, {STRNAME_sk_SK, sk_SK},
    // {STRNAME_sl_SI, sl_SI}, {STRNAME_so_DJ, so_DJ}, {STRNAME_so_KE, so_KE},
    // {STRNAME_so_SO, so_SO}, {STRNAME_sq_AL, sq_AL}, {STRNAME_st_ZA, st_ZA},
    // {STRNAME_sv_FI, sv_FI}, {STRNAME_sv_SE, sv_SE}, {STRNAME_tcy_IN, tcy_IN},
    // {STRNAME_tg_TJ, tg_TJ}, {STRNAME_th_TH, th_TH}, {STRNAME_tl_PH, tl_PH},
    // {STRNAME_tr_CY, tr_CY}, {STRNAME_tr_TR, tr_TR}, {STRNAME_uk_UA, uk_UA},
    // {STRNAME_uz_UZ, uz_UZ}, {STRNAME_wa_BE, wa_BE}, {STRNAME_xh_ZA, xh_ZA},
    // {STRNAME_yi_US, yi_US},
    {STRNAME_zh_CN, zh_CN},
    {STRNAME_zh_HK, zh_HK},
    // {STRNAME_zh_SG, zh_SG}, {STRNAME_zh_TW, zh_TW}, {STRNAME_zu_ZA, zu_ZA},
  };

  vector<tuple<const char *, Lang>> supported = {};

  const char *trailing_enc = ".UTF-8";
  size_t n_trailing_enc = 6;

  char *tmpbuf = (char *) alloca (6 + 6);
  for (const tuple<const char *, Lang> &t : lclist)
    {
      const char *strname = get<0> (t);
      size_t nchar = strlen (strname);
      memcpy (tmpbuf, strname, nchar);
      if (nchar == 5 && tmpbuf[2] == '-')
	{
	  tmpbuf[2] = '_';
	  memcpy (tmpbuf + 5, (void *) trailing_enc, n_trailing_enc);
	  tmpbuf[11] = 0;
	}
      else if (nchar == 6 && tmpbuf[3] == '-')
	{
	  tmpbuf[3] = '_';
	  memcpy (tmpbuf + 6, (void *) trailing_enc, n_trailing_enc);
	}
      if (setlocale (LC_ALL, tmpbuf) != nullptr)
	{
	  Lang lc = get<1> (t);

	  if (lc == PREFER_LANG)
	    {
	      HOST_LANG = lc;
	      return true;
	    }
	  else
	    {
	      supported.push_back (t);
	    }
	}
    }

  if (supported.size () > 0)
    {
      setlocale (LC_ALL, get<0> (supported[0]));
      HOST_LANG = get<1> (supported[0]);
      return true;
    }

  return false;
}

} // namespace tgf
