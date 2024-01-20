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

extern Lang
lang_from_cstr (const char *cstr)
{
  if (strlen (cstr) < 5)
    return Lang::unknown;

  if (strcmp (cstr, "aa_DJ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "af_ZA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "an_ES") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_AE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_BH") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_DZ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_EG") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_IQ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_JO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_KW") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_LB") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_LY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_MA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_OM") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_QA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_SA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_SD") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_SY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_TN") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ar_YE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ast_ES") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "be_BY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "bg_BG") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "bhb_IN") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "br_FR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "bs_BA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ca_AD") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ca_ES") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ca_FR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ca_IT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "cs_CZ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "cy_GB") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "da_DK") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_AT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_BE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_CH") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_DE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_IT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_LI") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "de_LU") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "el_CY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "el_GR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "en_AU") == 0)
    return Lang::en_AU;
  if (strcmp (cstr, "en_BW") == 0)
    return Lang::en_BW;
  if (strcmp (cstr, "en_CA") == 0)
    return Lang::en_CA;
  if (strcmp (cstr, "en_DK") == 0)
    return Lang::en_DK;
  if (strcmp (cstr, "en_GB") == 0)
    return Lang::en_GB;
  if (strcmp (cstr, "en_HK") == 0)
    return Lang::en_HK;
  if (strcmp (cstr, "en_IE") == 0)
    return Lang::en_IE;
  if (strcmp (cstr, "en_NZ") == 0)
    return Lang::en_NZ;
  if (strcmp (cstr, "en_PH") == 0)
    return Lang::en_PH;
  if (strcmp (cstr, "en_SC") == 0)
    return Lang::en_SC;
  if (strcmp (cstr, "en_SG") == 0)
    return Lang::en_SG;
  if (strcmp (cstr, "en_US") == 0)
    return Lang::en_US;
  if (strcmp (cstr, "en_ZA") == 0)
    return Lang::en_ZA;
  if (strcmp (cstr, "en_ZW") == 0)
    return Lang::en_ZW;
  if (strcmp (cstr, "es_AR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_BO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_CL") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_CO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_CR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_DO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_EC") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_ES") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_GT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_HN") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_MX") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_NI") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_PA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_PE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_PR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_PY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_SV") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_US") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_UY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "es_VE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "et_EE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "eu_ES") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "eu_FR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fi_FI") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fo_FO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fr_BE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fr_CA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fr_CH") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fr_FR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "fr_LU") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ga_IE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "gd_GB") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "gl_ES") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "gv_GB") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "he_IL") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "hr_HR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "hsb_DE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "hu_HU") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "id_ID") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "is_IS") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "it_CH") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "it_IT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ja_JP") == 0)
    return Lang::ja_JP;
  if (strcmp (cstr, "ka_GE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "kk_KZ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "kl_GL") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ko_KR") == 0)
    return Lang::ko_KR;
  if (strcmp (cstr, "ku_TR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "kw_GB") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "lg_UG") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "lt_LT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "lv_LV") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "mg_MG") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "mi_NZ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "mk_MK") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ms_MY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "mt_MT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "nb_NO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "nl_BE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "nl_NL") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "nn_NO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "oc_FR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "om_KE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "pl_PL") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "pt_BR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "pt_PT") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ro_RO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ru_RU") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "ru_UA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "sk_SK") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "sl_SI") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "so_DJ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "so_KE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "so_SO") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "sq_AL") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "st_ZA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "sv_FI") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "sv_SE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "tcy_IN") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "tg_TJ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "th_TH") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "tl_PH") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "tr_CY") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "tr_TR") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "uk_UA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "uz_UZ") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "wa_BE") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "xh_ZA") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "yi_US") == 0)
    return Lang::unknown;
  if (strcmp (cstr, "zh_CN") == 0)
    return Lang::zh_CN;
  if (strcmp (cstr, "zh_HK") == 0)
    return Lang::zh_HK;
  if (strcmp (cstr, "zh_SG") == 0)
    return Lang::zh_SG;
  if (strcmp (cstr, "zh_TW") == 0)
    return Lang::zh_TW;
  if (strcmp (cstr, "zu_ZA") == 0)
    return Lang::unknown;

  return Lang::unknown;
}

std::ostream &
operator<< (std::ostream &os, const Lang l)
{
  switch (l)
    {
    case Lang::aa_DJ:
      os << "aa_DJ";
      break;
    case Lang::af_ZA:
      os << "af_ZA";
      break;
    case Lang::an_ES:
      os << "an_ES";
      break;
    case Lang::ar_AE:
      os << "ar_AE";
      break;
    case Lang::ar_BH:
      os << "ar_BH";
      break;
    case Lang::ar_DZ:
      os << "ar_DZ";
      break;
    case Lang::ar_EG:
      os << "ar_EG";
      break;
    case Lang::ar_IQ:
      os << "ar_IQ";
      break;
    case Lang::ar_JO:
      os << "ar_JO";
      break;
    case Lang::ar_KW:
      os << "ar_KW";
      break;
    case Lang::ar_LB:
      os << "ar_LB";
      break;
    case Lang::ar_LY:
      os << "ar_LY";
      break;
    case Lang::ar_MA:
      os << "ar_MA";
      break;
    case Lang::ar_OM:
      os << "ar_OM";
      break;
    case Lang::ar_QA:
      os << "ar_QA";
      break;
    case Lang::ar_SA:
      os << "ar_SA";
      break;
    case Lang::ar_SD:
      os << "ar_SD";
      break;
    case Lang::ar_SY:
      os << "ar_SY";
      break;
    case Lang::ar_TN:
      os << "ar_TN";
      break;
    case Lang::ar_YE:
      os << "ar_YE";
      break;
    case Lang::ast_ES:
      os << "ast_ES";
      break;
    case Lang::be_BY:
      os << "be_BY";
      break;
    case Lang::bg_BG:
      os << "bg_BG";
      break;
    case Lang::bhb_IN:
      os << "bhb_IN";
      break;
    case Lang::br_FR:
      os << "br_FR";
      break;
    case Lang::bs_BA:
      os << "bs_BA";
      break;
    case Lang::ca_AD:
      os << "ca_AD";
      break;
    case Lang::ca_ES:
      os << "ca_ES";
      break;
    case Lang::ca_FR:
      os << "ca_FR";
      break;
    case Lang::ca_IT:
      os << "ca_IT";
      break;
    case Lang::cs_CZ:
      os << "cs_CZ";
      break;
    case Lang::cy_GB:
      os << "cy_GB";
      break;
    case Lang::da_DK:
      os << "da_DK";
      break;
    case Lang::de_AT:
      os << "de_AT";
      break;
    case Lang::de_BE:
      os << "de_BE";
      break;
    case Lang::de_CH:
      os << "de_CH";
      break;
    case Lang::de_DE:
      os << "de_DE";
      break;
    case Lang::de_IT:
      os << "de_IT";
      break;
    case Lang::de_LI:
      os << "de_LI";
      break;
    case Lang::de_LU:
      os << "de_LU";
      break;
    case Lang::el_CY:
      os << "el_CY";
      break;
    case Lang::el_GR:
      os << "el_GR";
      break;
    case Lang::en_AU:
      os << "en_AU";
      break;
    case Lang::en_BW:
      os << "en_BW";
      break;
    case Lang::en_CA:
      os << "en_CA";
      break;
    case Lang::en_DK:
      os << "en_DK";
      break;
    case Lang::en_GB:
      os << "en_GB";
      break;
    case Lang::en_HK:
      os << "en_HK";
      break;
    case Lang::en_IE:
      os << "en_IE";
      break;
    case Lang::en_NZ:
      os << "en_NZ";
      break;
    case Lang::en_PH:
      os << "en_PH";
      break;
    case Lang::en_SC:
      os << "en_SC";
      break;
    case Lang::en_SG:
      os << "en_SG";
      break;
    case Lang::en_US:
      os << "en_US";
      break;
    case Lang::en_ZA:
      os << "en_ZA";
      break;
    case Lang::en_ZW:
      os << "en_ZW";
      break;
    case Lang::es_AR:
      os << "es_AR";
      break;
    case Lang::es_BO:
      os << "es_BO";
      break;
    case Lang::es_CL:
      os << "es_CL";
      break;
    case Lang::es_CO:
      os << "es_CO";
      break;
    case Lang::es_CR:
      os << "es_CR";
      break;
    case Lang::es_DO:
      os << "es_DO";
      break;
    case Lang::es_EC:
      os << "es_EC";
      break;
    case Lang::es_ES:
      os << "es_ES";
      break;
    case Lang::es_GT:
      os << "es_GT";
      break;
    case Lang::es_HN:
      os << "es_HN";
      break;
    case Lang::es_MX:
      os << "es_MX";
      break;
    case Lang::es_NI:
      os << "es_NI";
      break;
    case Lang::es_PA:
      os << "es_PA";
      break;
    case Lang::es_PE:
      os << "es_PE";
      break;
    case Lang::es_PR:
      os << "es_PR";
      break;
    case Lang::es_PY:
      os << "es_PY";
      break;
    case Lang::es_SV:
      os << "es_SV";
      break;
    case Lang::es_US:
      os << "es_US";
      break;
    case Lang::es_UY:
      os << "es_UY";
      break;
    case Lang::es_VE:
      os << "es_VE";
      break;
    case Lang::et_EE:
      os << "et_EE";
      break;
    case Lang::eu_ES:
      os << "eu_ES";
      break;
    case Lang::eu_FR:
      os << "eu_FR";
      break;
    case Lang::fi_FI:
      os << "fi_FI";
      break;
    case Lang::fo_FO:
      os << "fo_FO";
      break;
    case Lang::fr_BE:
      os << "fr_BE";
      break;
    case Lang::fr_CA:
      os << "fr_CA";
      break;
    case Lang::fr_CH:
      os << "fr_CH";
      break;
    case Lang::fr_FR:
      os << "fr_FR";
      break;
    case Lang::fr_LU:
      os << "fr_LU";
      break;
    case Lang::ga_IE:
      os << "ga_IE";
      break;
    case Lang::gd_GB:
      os << "gd_GB";
      break;
    case Lang::gl_ES:
      os << "gl_ES";
      break;
    case Lang::gv_GB:
      os << "gv_GB";
      break;
    case Lang::he_IL:
      os << "he_IL";
      break;
    case Lang::hr_HR:
      os << "hr_HR";
      break;
    case Lang::hsb_DE:
      os << "hsb_DE";
      break;
    case Lang::hu_HU:
      os << "hu_HU";
      break;
    case Lang::id_ID:
      os << "id_ID";
      break;
    case Lang::is_IS:
      os << "is_IS";
      break;
    case Lang::it_CH:
      os << "it_CH";
      break;
    case Lang::it_IT:
      os << "it_IT";
      break;
    case Lang::ja_JP:
      os << "ja_JP";
      break;
    case Lang::ka_GE:
      os << "ka_GE";
      break;
    case Lang::kk_KZ:
      os << "kk_KZ";
      break;
    case Lang::kl_GL:
      os << "kl_GL";
      break;
    case Lang::ko_KR:
      os << "ko_KR";
      break;
    case Lang::ku_TR:
      os << "ku_TR";
      break;
    case Lang::kw_GB:
      os << "kw_GB";
      break;
    case Lang::lg_UG:
      os << "lg_UG";
      break;
    case Lang::lt_LT:
      os << "lt_LT";
      break;
    case Lang::lv_LV:
      os << "lv_LV";
      break;
    case Lang::mg_MG:
      os << "mg_MG";
      break;
    case Lang::mi_NZ:
      os << "mi_NZ";
      break;
    case Lang::mk_MK:
      os << "mk_MK";
      break;
    case Lang::ms_MY:
      os << "ms_MY";
      break;
    case Lang::mt_MT:
      os << "mt_MT";
      break;
    case Lang::nb_NO:
      os << "nb_NO";
      break;
    case Lang::nl_BE:
      os << "nl_BE";
      break;
    case Lang::nl_NL:
      os << "nl_NL";
      break;
    case Lang::nn_NO:
      os << "nn_NO";
      break;
    case Lang::oc_FR:
      os << "os";
      break;
    case Lang::om_KE:
      os << "os";
      break;
    case Lang::pl_PL:
      os << "pl_PL";
      break;
    case Lang::pt_BR:
      os << "pt_BR";
      break;
    case Lang::pt_PT:
      os << "pt_PT";
      break;
    case Lang::ro_RO:
      os << "ro_RO";
      break;
    case Lang::ru_RU:
      os << "ru_RU";
      break;
    case Lang::ru_UA:
      os << "ru_UA";
      break;
    case Lang::sk_SK:
      os << "sk_SK";
      break;
    case Lang::sl_SI:
      os << "sl_SI";
      break;
    case Lang::so_DJ:
      os << "so_DJ";
      break;
    case Lang::so_KE:
      os << "so_KE";
      break;
    case Lang::so_SO:
      os << "so_SO";
      break;
    case Lang::sq_AL:
      os << "sq_AL";
      break;
    case Lang::st_ZA:
      os << "st_ZA";
      break;
    case Lang::sv_FI:
      os << "sv_FI";
      break;
    case Lang::sv_SE:
      os << "sv_SE";
      break;
    case Lang::tcy_IN:
      os << "tcy_IN";
      break;
    case Lang::tg_TJ:
      os << "tg_TJ";
      break;
    case Lang::th_TH:
      os << "th_TH";
      break;
    case Lang::tl_PH:
      os << "tl_PH";
      break;
    case Lang::tr_CY:
      os << "tr_CY";
      break;
    case Lang::tr_TR:
      os << "tr_TR";
      break;
    case Lang::uk_UA:
      os << "uk_UA";
      break;
    case Lang::uz_UZ:
      os << "uz_UZ";
      break;
    case Lang::wa_BE:
      os << "wa_BE";
      break;
    case Lang::xh_ZA:
      os << "xh_ZA";
      break;
    case Lang::yi_US:
      os << "yi_US";
      break;
    case Lang::zh_CN:
      os << "zh_CN";
      break;
    case Lang::zh_HK:
      os << "zh_HK";
      break;
    case Lang::zh_SG:
      os << "zh_SG";
      break;
    case Lang::zh_TW:
      os << "zh_TW";
      break;
    case Lang::zu_ZA:
      os << "zu_ZA";
      break;

    default:
      os << "???_???";
      break;
    }

  return os;
}

bool
try_ensure_locale ()
{
  using namespace std;

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

  vector<Lang> supported = {};

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
	    supported.push_back (lc);
	  }
      }

  if (supported.size () > 0)
    {
      HOST_LANG = supported[0];
      return true;
    }

  return false;
}

} // namespace tgf
