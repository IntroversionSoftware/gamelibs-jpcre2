/**@file testme.cpp
 * Examples of using MatchEvaluator to perform replace.
 * 
 * @include testme.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */
//~ #define NDEBUG
#include <iostream>
#include "jpcre2.hpp"
#include "test.hpp"
#if __cplusplus >= 201103L
#include <unordered_map>
#endif

#if __cplusplus >= 201103L
//In >=C++11 you can pass an optional template parameter to select the map container that will be
//used for MapNas and MapNtn instead of std::map
typedef jpcre2::select<char, std::unordered_map> jp;
#else
typedef jpcre2::select<char> jp;
#endif

typedef jp::String String;

String toString (int x){
    char buf[128];
    int written = std::sprintf(buf, "%d", x);
    JPCRE2_ASSERT(written > 0, "IOError: Failed to write into buffer during int to string conversion.");
    return String(buf);
}

String callback0(void*, void*, void*){
    return "\nw: $2\ts: $3\td: $4\n";
}

String callback1(const jp::NumSub& vec_num, void*, void*){
    return "("+jp::String(VEC_NUM[0])+")";
}

String callback2(void*, const jp::MapNas& m2, void*){
    return "("+jp::String(m2.at("total"))+")";
}

String callback3(const jp::NumSub& vec_num,const jp::MapNas& m2, void*){
    jp::MapNas mn2 = m2;
    return "("+jp::String(VEC_NUM[0])+"/"+jp::String(mn2["total"])+")";
}

String callback4(void*, void*, const jp::MapNtN& m3){
    jp::MapNtN mn3 = m3;
    return "("+toString(mn3["total"])+")";
}

String callback5(const jp::NumSub& vec_num, void*, const jp::MapNtN& m3){
    jp::MapNtN mn = m3;
    return "("+jp::String(VEC_NUM[0])+"/"+toString(mn["total"])+")";
}

String callback6(void*, const jp::MapNas& m2, const jp::MapNtn& m3){
    jp::MapNas mn2 = m2;
    jp::MapNtN mn3 = m3;
    return "("+jp::String(mn2["total"])+"/"+toString(mn3["total"])+")";
}

String callback7(const jp::NumSub& vec_num, const jp::MapNas& m2, const jp::MapNtn& m3){
    jp::MapNas mn2 = m2;
    jp::MapNtN mn3 = m3;
    return "("+jp::String(VEC_NUM[0])+"/"+jp::String(mn2["total"])+"/"+toString(mn3["total"])+"/$0)";
}

//The following is an example how you can use start_offset and 
//end_offset inside callback
jpcre2::VecOff const* start_offset, *end_offset;
size_t offset_count = 0;
String callback_using_offset(const jp::NumSub& vec_num, void*, void* ){
    size_t count = offset_count++;
    return "(m[0]: "+ jp::String(VEC_NUM[0]) + "/" + "start_offset: " + toString((*start_offset)[count]) + "/end_offset: " +  toString((*end_offset)[count]);
}

int main(){
    jp::Regex re("(?<total>(?<w>\\w+)(?<s>\\s+)(?<d>\\d+))", "n");
    jp::RegexReplace rr(&re);
    
    String s3 = "I am a string 879879 fdsjkll ১ ২ ৩ ৪ অ আ ক খ গ ঘ";
    
    
    rr.setSubject(s3)
      .setPcre2Option(PCRE2_SUBSTITUTE_GLOBAL);
      
    rr.setSubject(s3); //this is allowed too, makes a copy.
    
    #if __cplusplus >= 201103L
    //example with lambda
    std::cout<<"\n\n### Lambda\n"<<rr.nreplace(
                jp::MatchEvaluator(
                    [](const jp::NumSub& vec_num, const jp::MapNas& m2, void*){
                        return "("+jp::String(VEC_NUM[0])+"/"+jp::String(m2.at("total"))+")";
                    }
                ));
    #endif
    
    std::cout<<"\n\n### 0\n"<<rr.nreplace(jp::MatchEvaluator(callback0));
    std::cout<<"\n\n### 1\n"<<rr.nreplace(jp::MatchEvaluator(callback1));
    std::cout<<"\n\n### 2\n"<<rr.nreplace(jp::MatchEvaluator(callback2));
    std::cout<<"\n\n### 3\n"<<rr.nreplace(jp::MatchEvaluator(callback3));
    std::cout<<"\n\n### 4\n"<<rr.nreplace(jp::MatchEvaluator(callback4));
    std::cout<<"\n\n### 5\n"<<rr.nreplace(jp::MatchEvaluator(callback5));
    std::cout<<"\n\n### 6\n"<<rr.nreplace(jp::MatchEvaluator(callback6));
    std::cout<<"\n\n### 7\n"<<rr.nreplace(jp::MatchEvaluator(callback7));
    
    //MatchEvaluator itself has an nreplace() function:
    std::cout<<"\n\n### 7 Calling directly MatchEvaluator::nreplace()\n"
             <<jp::MatchEvaluator(callback7)
                                 .setSubject(s3)
                                 .setRegexObject(&re) 
                                 .setFindAll()
                                 .nreplace();
    //note the setFindAll() in above, without it, only single replacement would occur because there would be only one match.
    
    
        /* *****************************************************************
         * Re-using same MatchEvaluator for different replace operation
         * by using existing match data with different callback function:
         * ****************************************************************/
    
    jp::MatchEvaluator cme(jp::callback::fill);
    //~ //perform a match to populate all the vectos with match data.
    cme.setSubject(s3).setRegexObject(&re).setFindAll().match();
    
    std::cout<<"\n\n###### Re-using existing match data of MatchEvaluator:";
    std::cout<<"\n\n### callback0: \n"<<cme.setCallback(callback0).nreplace();      //this one performs the match again (redundant).
    std::cout<<"\n\n### callback1: \n"<<cme.setCallback(callback1).nreplace(false); //this one and all the following
    std::cout<<"\n\n### callback2: \n"<<cme.setCallback(callback2).nreplace(false); //uses existing match data
    std::cout<<"\n\n### callback3: \n"<<cme.setCallback(callback3).nreplace(false); //from the previous match
    std::cout<<"\n\n### callback4: \n"<<cme.setCallback(callback4).nreplace(false);
    std::cout<<"\n\n### callback5: \n"<<cme.setCallback(callback5).nreplace(false);
    std::cout<<"\n\n### callback6: \n"<<cme.setCallback(callback6).nreplace(false);
    std::cout<<"\n\n### callback7: \n"<<cme.setCallback(callback7).setFindAll(false).nreplace(false);
    
    //note the 'false' in the above nreplace() functions, it says 'do not perform a new match' i.e 'use previous match data'
    
    cme.reset();
    cme.setSubject(s3).setRegexObject(&re).setFindAll().match();
    
    //the following nreplace() performs a new match populating NumSub and MapNas because of callback3:
    std::cout<<"\n\n### callback3: \n"<<cme.setCallback(callback3).nreplace();
    
    //Now you can use either one of callback1, callback2 or callback3 without performing a new match,
    //as the required vectors NumSub and MapNas are both populated.
    std::cout<<"\n\n### callback2: \n"<<cme.setCallback(callback2).nreplace(false);
    
    //jp::callback::eraseFill function initiates all vectors (kinda like callback7). Thus, the following
    //populates all vectors. It erases the matched part/s from the subject string.
    std::cout<<"\n\n### default callback: \n"<<cme.setCallback(jp::callback::eraseFill).nreplace();
    //After populating all vectors, you can use any type of callback without performing the match again.
    
    
    //The following (uncomment if you wanna test) will give you assertion failure, because the callback1 only populates NumSub vector,
    //but callback2 requires pre-exisiting (due to the 'false' argument to nreplace()) MapNas data:
    cme.reset().setSubject(s3).setRegexObject(&re).setFindAll().setCallback(callback1).nreplace();
    //~ std::cout<<"\n\n### callback2: \n"<<cme.setCallback(callback2).nreplace(false); //Assertion failure.
    



        /* *****************************************************************
         *                 PCRE2 compatible replace
         * MatchEvaluator has a replace() funtion that uses pcre2_substitute
         * ****************************************************************/
     
     
     
    std::cout<<"\n####replace: \n"<<cme.setCallback(callback0).replace();
    //The string returned by callback0: "\nw: $2\ts: $3\td: $4\n" which is interpreted by PCRE2 substitue function.
    //thus allow all options provided by PCRE2 library.
    //Short note: 
    // * replace() funtion is for PCRE2 compatible substitute.
    // * nreplace() is JPCRE2 native replace function.
    
    std::cout<<"\ncallback7: \n"<<cme.setCallback(callback7).setFindAll(false).replace();
    
    //this can also be done with RegexReplace::replace()
    std::cout<<"\ncallback7: \n"<<rr.replace(cme); //rr is set with global match. this replace always performs a new match.
    
    std::cout<<"\n\n### Doing another array of replacement with PCRE2 compatible replace:\n";
    std::cout<<"\n\n### callback0: \n"<<cme.setCallback(callback0).replace();      //this one performs the match again (redundant).
    std::cout<<"\n\n### callback1: \n"<<cme.setCallback(callback1).replace(false); //this one and all the following
    std::cout<<"\n\n### callback2: \n"<<cme.setCallback(callback2).replace(false); //uses existing match data
    std::cout<<"\n\n### callback3: \n"<<cme.setCallback(callback3).replace(false); //from the previous match
    std::cout<<"\n\n### callback4: \n"<<cme.setCallback(callback4).replace(false);
    std::cout<<"\n\n### callback5: \n"<<cme.setCallback(callback5).replace(false);
    std::cout<<"\n\n### callback6: \n"<<cme.setCallback(callback6).replace(false);
    std::cout<<"\n\n### callback7: \n"<<cme.setCallback(callback7).setFindAll(false).replace(false);
    
    
    /* *********************************************************************
     * The following is an example how you can use start_offset and 
     * end_offset inside callback
     * *********************************************************************/
    
    start_offset = cme.getMatchStartOffsetVector();
    end_offset = cme.getMatchEndOffsetVector();
    std::cout<<"\n\n### callback_using_offset: \n"<<cme.setCallback(callback_using_offset).replace();
    
    #if __cplusplus >= 201103L
    //using lambda
    jpcre2::VecOff const * so = cme.getMatchStartOffsetVector();
    jpcre2::VecOff const * eo = cme.getMatchEndOffsetVector();
    size_t off_count = 0;
    cme.setCallback(
        [&](const jp::NumSub& vec_num, void*, void*){
            size_t count = off_count++;
            return "(m[0]: "+ jp::String(VEC_NUM[0]) + "/" + "start_offset: " + toString((*so)[count]) + "/end_offset: " +  toString((*eo)[count]) + ")";
        }
    );
    std::cout<<"\n\n### lambda_callback_using_offset: \n"<<cme.replace();
    #endif
    
    
    /* *****************************************************************
     *              Some random sanity checks
     * ****************************************************************/
    
    cme.setCallback(callback1);
    std::cout<<"\n\nanother: "<<
    jp::Regex("[\\d\\s]*\\K\\w+").initReplace().setSubject("fds 324 fd34").setModifier("g").replace(cme);
    
    
    jp::Regex("[\\d\\s]*\\K\\w+").initReplace()
                                 .setSubject("fds 324 fd34")
                                 .setModifier("g")
                                 .changePcre2Option(PCRE2_SUBSTITUTE_OVERFLOW_LENGTH, false) //this is wrong, will perform no replace if buffer is not enough
                                 .replace(cme);
    
    
    rr.setRegexObject(0);
    JPCRE2_ASSERT(rr.nreplace(jp::MatchEvaluator(callback1))==s3,"InvalidResult"); 
    
    rr.setRegexObject(&re).setPcre2Option(0).nreplace(jp::MatchEvaluator(callback2));
    

    jp::MatchEvaluator me1(&callback0);
    
    jp::MatchEvaluator me2 = me1;
    jp::MatchEvaluator me3 = me2;
    
    rr.setRegexObject(&re).setPcre2Option(0).nreplace(me1);
    
    
    me2 = jp::MatchEvaluator(callback2);
    me1 = me2;
    rr.nreplace(me1);
    me2 = me1;
    me1 = jp::MatchEvaluator(callback3);
    rr.nreplace(me1);
    me3 = me1;
    me1 = jp::MatchEvaluator(callback4);
    
    rr.nreplace(me1);
    me2 = me1;
    #if __cplusplus >= 201103L
    me2 = std::move(me1);
    jp::MatchEvaluator mem(std::move(me2));
    #endif
    me1 = me3;
    me1 = jp::MatchEvaluator(callback5);
    me1 = jp::MatchEvaluator(callback6);
    me1 = jp::MatchEvaluator(callback7);
    me1 = jp::MatchEvaluator(callback1);
    
    me1.resetErrors().resetMatchData().reset().clear().setBufferSize(3).getBufferSize();
    me1.setModifier("g").addModifier("").changeModifier("",true);
    me1.setPcre2Option(0).addPcre2Option(0).changePcre2Option(0, !0);
    me1.setJpcre2Option(0).addJpcre2Option(0).changeJpcre2Option(0,!0);
    me1.setStartOffset(0).setMatchContext(0);
    me1.setRegexObject(&re).setSubject(s3);
    me1.setCallback(jp::callback::fill).nreplace();
    me1.setCallback(jp::callback::eraseFill).nreplace();
    me1.setCallback(jp::callback::erase).nreplace(true, 0, 0);
    size_t counter;
    me1.setCallback(jp::callback::erase).nreplace(true, 0, &counter);

    return 0;
}
