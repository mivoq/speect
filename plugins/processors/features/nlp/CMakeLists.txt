######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for nlp feature processors plug-ins                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(segment_duration)
add_subdirectory(segment_end)
add_subdirectory(segment_mid)
add_subdirectory(segment_pos_syl)
add_subdirectory(segment_pos_syl_rev)
add_subdirectory(segment_start)
add_subdirectory(syllable_break)
add_subdirectory(syllable_duration)
add_subdirectory(syllable_end)
add_subdirectory(syllable_num_phones)
add_subdirectory(syllable_pos_type)
add_subdirectory(syllable_pos_word)
add_subdirectory(syllable_pos_word_rev)
add_subdirectory(syllable_pos_phrase)
add_subdirectory(syllable_pos_phrase_rev)
add_subdirectory(syllable_start)
add_subdirectory(syllable_vowel)
add_subdirectory(syllable_stress_in)
add_subdirectory(syllable_stress_out)
add_subdirectory(syllable_accent_in)
add_subdirectory(syllable_accent_out)
add_subdirectory(syllable_stress_all_in)
add_subdirectory(syllable_stress_all_out)
add_subdirectory(syllable_accent_all_in)
add_subdirectory(syllable_accent_all_out)
add_subdirectory(word_break)
add_subdirectory(word_duration)
add_subdirectory(word_end)
add_subdirectory(word_num_syls)
add_subdirectory(word_pos_phrase)
add_subdirectory(word_pos_phrase_rev)
add_subdirectory(word_start)
add_subdirectory(word_content_in)
add_subdirectory(word_content_out)
add_subdirectory(word_content_all_in)
add_subdirectory(word_content_all_out)
add_subdirectory(phrase_num_syls)
add_subdirectory(phrase_num_words)
add_subdirectory(phrase_pos_utt)
add_subdirectory(phrase_pos_utt_rev)
add_subdirectory(utt_num_syls)
add_subdirectory(utt_num_words)
add_subdirectory(utt_num_phrases)

if ((HTS_ENGINE_INCLUDE AND HTS_ENGINE_LIB) OR (HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102) OR (HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103))
  add_subdirectory(hts_labels)
endif((HTS_ENGINE_INCLUDE AND HTS_ENGINE_LIB) OR (HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102) OR (HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103))
