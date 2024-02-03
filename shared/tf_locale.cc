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
// const char *STRNAME_en_ZW = "en-ZW";
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
  // if (strcmp (cstr, STRNAME_en_ZW) == 0)
  //   return Lang::en_ZW;
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
    // case Lang::en_ZW:
    //   return STRNAME_en_ZW;
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
  vector<tuple<string, Lang>> lclist = {
    // {"C.UTF-8", unknown}, // we dont need this
    {"aa_DJ.UTF-8", aa_DJ},   {"af_ZA.UTF-8", af_ZA}, {"an_ES.UTF-8", an_ES},
    {"ar_AE.UTF-8", ar_AE},   {"ar_BH.UTF-8", ar_BH}, {"ar_DZ.UTF-8", ar_DZ},
    {"ar_EG.UTF-8", ar_EG},   {"ar_IQ.UTF-8", ar_IQ}, {"ar_JO.UTF-8", ar_JO},
    {"ar_KW.UTF-8", ar_KW},   {"ar_LB.UTF-8", ar_LB}, {"ar_LY.UTF-8", ar_LY},
    {"ar_MA.UTF-8", ar_MA},   {"ar_OM.UTF-8", ar_OM}, {"ar_QA.UTF-8", ar_QA},
    {"ar_SA.UTF-8", ar_SA},   {"ar_SD.UTF-8", ar_SD}, {"ar_SY.UTF-8", ar_SY},
    {"ar_TN.UTF-8", ar_TN},   {"ar_YE.UTF-8", ar_YE}, {"ast_ES.UTF-8", ast_ES},
    {"be_BY.UTF-8", be_BY},   {"bg_BG.UTF-8", bg_BG}, {"bhb_IN.UTF-8", bhb_IN},
    {"br_FR.UTF-8", br_FR},   {"bs_BA.UTF-8", bs_BA}, {"ca_AD.UTF-8", ca_AD},
    {"ca_ES.UTF-8", ca_ES},   {"ca_FR.UTF-8", ca_FR}, {"ca_IT.UTF-8", ca_IT},
    {"cs_CZ.UTF-8", cs_CZ},   {"cy_GB.UTF-8", cy_GB}, {"da_DK.UTF-8", da_DK},
    {"de_AT.UTF-8", de_AT},   {"de_BE.UTF-8", de_BE}, {"de_CH.UTF-8", de_CH},
    {"de_DE.UTF-8", de_DE},   {"de_IT.UTF-8", de_IT}, {"de_LI.UTF-8", de_LI},
    {"de_LU.UTF-8", de_LU},   {"el_CY.UTF-8", el_CY}, {"el_GR.UTF-8", el_GR},
    {"en_HK.UTF-8", en_HK},   {"en_AU.UTF-8", en_AU}, {"en_BW.UTF-8", en_BW},
    {"en_CA.UTF-8", en_CA},   {"en_DK.UTF-8", en_DK}, {"en_GB.UTF-8", en_GB},
    {"en_HK.UTF-8", en_HK},   {"en_IE.UTF-8", en_IE}, {"en_NZ.UTF-8", en_NZ},
    {"en_PH.UTF-8", en_PH},   {"en_SC.UTF-8", en_SC}, {"en_SG.UTF-8", en_SG},
    {"en_US.UTF-8", en_US},   {"en_ZA.UTF-8", en_ZA}, {"en_ZW.UTF-8", en_ZW},
    {"es_AR.UTF-8", es_AR},   {"es_BO.UTF-8", es_BO}, {"es_CL.UTF-8", es_CL},
    {"es_CO.UTF-8", es_CO},   {"es_CR.UTF-8", es_CR}, {"es_DO.UTF-8", es_DO},
    {"es_EC.UTF-8", es_EC},   {"es_ES.UTF-8", es_ES}, {"es_GT.UTF-8", es_GT},
    {"es_HN.UTF-8", es_HN},   {"es_MX.UTF-8", es_MX}, {"es_NI.UTF-8", es_NI},
    {"es_PA.UTF-8", es_PA},   {"es_PE.UTF-8", es_PE}, {"es_PR.UTF-8", es_PR},
    {"es_PY.UTF-8", es_PY},   {"es_SV.UTF-8", es_SV}, {"es_US.UTF-8", es_US},
    {"es_UY.UTF-8", es_UY},   {"es_VE.UTF-8", es_VE}, {"et_EE.UTF-8", et_EE},
    {"eu_ES.UTF-8", eu_ES},   {"eu_FR.UTF-8", eu_FR}, {"fi_FI.UTF-8", fi_FI},
    {"fo_FO.UTF-8", eu_FR},   {"fr_BE.UTF-8", fr_BE}, {"fr_CA.UTF-8", fr_CA},
    {"fr_CH.UTF-8", fr_CH},   {"fr_FR.UTF-8", fr_FR}, {"fr_LU.UTF-8", fr_LU},
    {"ga_IE.UTF-8", ga_IE},   {"gd_GB.UTF-8", gd_GB}, {"gl_ES.UTF-8", gl_ES},
    {"gv_GB.UTF-8", gv_GB},   {"he_IL.UTF-8", he_IL}, {"hr_HR.UTF-8", hr_HR},
    {"hsb_DE.UTF-8", hsb_DE}, {"hu_HU.UTF-8", hu_HU}, {"id_ID.UTF-8", id_ID},
    {"is_IS.UTF-8", is_IS},   {"it_CH.UTF-8", it_CH}, {"it_IT.UTF-8", it_IT},
    {"ja_JP.UTF-8", ja_JP},   {"ka_GE.UTF-8", ka_GE}, {"kk_KZ.UTF-8", kk_KZ},
    {"kl_GL.UTF-8", kl_GL},   {"ko_KR.UTF-8", ko_KR}, {"ku_TR.UTF-8", ku_TR},
    {"kw_GB.UTF-8", kw_GB},   {"lg_UG.UTF-8", lg_UG}, {"lt_LT.UTF-8", lt_LT},
    {"lv_LV.UTF-8", lv_LV},   {"mg_MG.UTF-8", mg_MG}, {"mi_NZ.UTF-8", mi_NZ},
    {"mk_MK.UTF-8", mk_MK},   {"ms_MY.UTF-8", ms_MY}, {"mt_MT.UTF-8", mt_MT},
    {"nb_NO.UTF-8", nb_NO},   {"nl_BE.UTF-8", nl_BE}, {"nl_NL.UTF-8", nl_NL},
    {"nn_NO.UTF-8", nn_NO},   {"oc_FR.UTF-8", oc_FR}, {"om_KE.UTF-8", om_KE},
    {"pl_PL.UTF-8", pl_PL},   {"pt_BR.UTF-8", pt_BR}, {"pt_PT.UTF-8", pt_PT},
    {"ro_RO.UTF-8", ro_RO},   {"ru_RU.UTF-8", ru_RU}, {"ru_UA.UTF-8", ru_UA},
    {"sk_SK.UTF-8", sk_SK},   {"sl_SI.UTF-8", sl_SI}, {"so_DJ.UTF-8", so_DJ},
    {"so_KE.UTF-8", so_KE},   {"so_SO.UTF-8", so_SO}, {"sq_AL.UTF-8", sq_AL},
    {"st_ZA.UTF-8", st_ZA},   {"sv_FI.UTF-8", sv_FI}, {"sv_SE.UTF-8", sv_SE},
    {"tcy_IN.UTF-8", tcy_IN}, {"tg_TJ.UTF-8", tg_TJ}, {"th_TH.UTF-8", th_TH},
    {"tl_PH.UTF-8", tl_PH},   {"tr_CY.UTF-8", tr_CY}, {"tr_TR.UTF-8", tr_TR},
    {"uk_UA.UTF-8", uk_UA},   {"uz_UZ.UTF-8", uz_UZ}, {"wa_BE.UTF-8", wa_BE},
    {"xh_ZA.UTF-8", xh_ZA},   {"yi_US.UTF-8", yi_US}, {"zh_CN.UTF-8", zh_CN},
    {"zh_HK.UTF-8", zh_HK},   {"zh_SG.UTF-8", zh_SG}, {"zh_TW.UTF-8", zh_TW},
    {"zu_ZA.UTF-8", zu_ZA},
  };

  vector<tuple<string, Lang>> supported = {};

  for (const tuple<string, Lang> &t : lclist)
    if (setlocale (LC_ALL, get<0> (t).c_str ()) != nullptr)
      {
	auto lc = get<1> (t);

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

  if (supported.size () > 0)
    {
      setlocale (LC_ALL, get<0> (supported[0]).c_str ());
      HOST_LANG = get<1> (supported[0]);
      return true;
    }

  return false;
}

} // namespace tgf
