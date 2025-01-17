/**@file testio.cpp
 * A test suit to check for code validity.
 * 
 * Goals:
 * 
 * 1. Check Validity of output for different inputs
 * 2. Memory check
 * 3. Segfault check
 * 4. coverage
 * 
 * This program will exit with 0 if all test passes, otherwise exit with
 * a non-zero status when error occurs.
 * 
 * @include testio.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */
#include <iostream>
#include "jpcre2.hpp"
#include <cassert>

typedef jpcre2::select<char> jpc;
typedef jpcre2::select<wchar_t> jpw;
#if __cplusplus >= 201103L
typedef jpcre2::select<char16_t> jp16;
typedef jpcre2::select<char32_t> jp32;
#endif

int main(){
    ////////////////////// Check with std::string //////////////////////
    jpc::Regex rec;
    jpc::VecNum vec_numc;
    jpc::VecNas vec_nasc;
    jpc::VecNtN vec_ntnc;
    jpc::RegexMatch rmc;
    jpc::RegexReplace rrc;
    
    rec.setPattern("(?<digit>\\d)").compile();
    rmc.setRegexObject(&rec)
       .setNumberedSubstringVector(&vec_numc)
       .setNamedSubstringVector(&vec_nasc)
       .setNameToNumberMapVector(&vec_ntnc)
       .setSubject("123456789")
       .setModifier("g");
    
    //check for validity of match
    assert(rmc.match() == 9);
    assert(vec_numc[8][0] == "9");
    assert(vec_numc[8][1] == "9");
    assert(vec_nasc[8]["digit"] == "9");
    assert(vec_ntnc[8]["digit"] == 1);
    
    //check for validity of replace
    assert(rec.replace("123456789", "d$1") == "d123456789");
    size_t counter;
    assert(rec.replace("123456789", "d$1", "g") == "d1d2d3d4d5d6d7d8d9");
    assert(rec.replace("123456789", "d$1", "g", &counter) == "d1d2d3d4d5d6d7d8d9");
    JPCRE2_ASSERT(counter == 9, "replace counter gave wrong result");
    
#if 0
    rrc.setSubject("123456789").setRegexObject(&rec).setReplaceWith("d$0");
    rrc.replace();
    JPCRE2_ASSERT(rrc.preplace() == 1, "replace counter gave wrong result");
    std::string s1 = "123456789";
    rrc.setSubject(&s1).setRegexObject(&rec).setReplaceWith("d$0");
    JPCRE2_ASSERT(rrc.preplace() == 1, "replace counter gave wrong result");
    JPCRE2_ASSERT(s1 == "d123456789", "preplace didn't modify the string in-place");
    JPCRE2_ASSERT(rrc.getLastReplaceCount() == 1, "replace counter gave wrong result");
    rrc.setRegexObject(0).replace();
    JPCRE2_ASSERT(rrc.getLastReplaceCount() == 0, "replace counter gave wrong result");
    
    rec.compile("\\w*\\K\\w*");
    jpc::MatchEvaluator(&rec).setSubject("----fdsjflfsd8fds68").nreplace();
    
    jpc::MatchEvaluator mec; //erase callback
    rec.compile("\\d");
    rrc.setRegexObject(&rec).setSubject(&s1).setModifier("g");
    JPCRE2_ASSERT(rrc.preplace(mec) == 9, "Invalid replace count with match evaluator");
    JPCRE2_ASSERT(s1 == "d", "Match evaluator with preplace did not work");
    std::string rs = "d";
    JPCRE2_ASSERT(rec.preplace("123456789", "d", "g") == 9, "Error in rec.preplace");
    JPCRE2_ASSERT(rec.preplace("123456789", &rs, "g") == 9, "Error in rec.preplace");
    s1 = "123456789";
    JPCRE2_ASSERT(rec.preplace(&s1, "d", "g") == 9, "Error in rec.preplace");
    JPCRE2_ASSERT(rec.preplace(&s1, &rs, "g") == 0, "Error in rec.preplace");
    JPCRE2_ASSERT(s1 == "ddddddddd", "Error in rec.preplace");
#endif 
    
    ////////////////////////////////////////////////////////////////////
    
    //////////////////////// Check with std::wstring ///////////////////
    jpw::Regex rew;
    jpw::VecNum vec_numw;
    jpw::VecNas vec_nasw;
    jpw::VecNtN vec_ntnw;
    jpw::RegexMatch rmw;
    
    rew.setPattern(L"(?<digit>\\d)").compile();
    rmw.setRegexObject(&rew)
       .setNumberedSubstringVector(&vec_numw)
       .setNamedSubstringVector(&vec_nasw)
       .setNameToNumberMapVector(&vec_ntnw)
       .setSubject(L"123456789")
       .setModifier("g");
    
    //check for validity of match
    assert(rmw.match() == 9);
    assert(vec_numw[8][0] == L"9");
    assert(vec_numw[8][1] == L"9");
    assert(vec_nasw[8][L"digit"] == L"9");
    assert(vec_ntnw[8][L"digit"] == 1);
    
    //check for validity of replace
    assert(rew.replace(L"123456789", L"d$1") == L"d123456789");
    assert(rew.replace(L"123456789", L"d$1", "g") == L"d1d2d3d4d5d6d7d8d9");
    assert(rew.replace(L"123456789", L"d$1", "g", &counter) == L"d1d2d3d4d5d6d7d8d9");
    JPCRE2_ASSERT(counter == 9, "replace counter gave wrong result");
    ////////////////////////////////////////////////////////////////////
    
    #if __cplusplus >= 201103L
    //////////////////////// Check with std::u16string ///////////////////
    jp16::Regex re16;
    jp16::VecNum vec_num16;
    jp16::VecNas vec_nas16;
    jp16::VecNtN vec_ntn16;
    jp16::RegexMatch rm16;
    
    re16.setPattern(u"(?<digit>\\d)").compile();
    rm16.setRegexObject(&re16)
        .setNumberedSubstringVector(&vec_num16)
        .setNamedSubstringVector(&vec_nas16)
        .setNameToNumberMapVector(&vec_ntn16)
        .setSubject(u"123456789")
        .setModifier("g");
    
    //check for validity of match
    assert(rm16.match() == 9);
    assert(vec_num16[8][0] == u"9");
    assert(vec_num16[8][1] == u"9");
    assert(vec_nas16[8][u"digit"] == u"9");
    assert(vec_ntn16[8][u"digit"] == 1);
    
    //check for validity of replace
    assert(re16.replace(u"123456789", u"d$1") == u"d123456789");
    assert(re16.replace(u"123456789", u"d$1", "g") == u"d1d2d3d4d5d6d7d8d9");
    assert(re16.replace(u"123456789", u"d$1", "g", &counter) == u"d1d2d3d4d5d6d7d8d9");
    JPCRE2_ASSERT(counter == 9, "replace counter gave wrong result");
    ////////////////////////////////////////////////////////////////////
    
    //////////////////////// Check with std::u32string ///////////////////
    jp32::Regex re32;
    jp32::VecNum vec_num32;
    jp32::VecNas vec_nas32;
    jp32::VecNtN vec_ntn32;
    jp32::RegexMatch rm32;
    
    re32.setPattern(U"(?<digit>\\d)").compile();
    rm32.setRegexObject(&re32)
        .setNumberedSubstringVector(&vec_num32)
        .setNamedSubstringVector(&vec_nas32)
        .setNameToNumberMapVector(&vec_ntn32)
        .setSubject(U"123456789")
        .setModifier("g");
    
    //check for validity of match
    assert(rm32.match() == 9);
    assert(vec_num32[8][0] == U"9");
    assert(vec_num32[8][1] == U"9");
    assert(vec_nas32[8][U"digit"] == U"9");
    assert(vec_ntn32[8][U"digit"] == 1);
    
    //check for validity of replace
    assert(re32.replace(U"123456789", U"d$1") == U"d123456789");
    assert(re32.replace(U"123456789", U"d$1", "g") == U"d1d2d3d4d5d6d7d8d9");
    assert(re32.replace(U"123456789", U"d$1", "g", &counter) == U"d1d2d3d4d5d6d7d8d9");
    JPCRE2_ASSERT(counter == 9, "replace counter gave wrong result");
    ////////////////////////////////////////////////////////////////////
    #endif
    
    std::string s("");
    jpcre2::Modifier md1;
    jpcre2::Modifier md2(s);
    md1.str();
    md2.c_str();
    
    
    #if __cplusplus >= 201103L
    jpc::RegexMatch rmc1(std::move(rmc));
    jpc::Regex rec1(std::move(rec));
    jpc::RegexReplace rrc1(std::move(rrc));
    #endif
    
    
    return 0;
}
