add_test(
  NAME "Italian-syllabification"
  COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/syll_test.test" "${CMAKE_CURRENT_SOURCE_DIR}/configurations/it-sample/voice.json" "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/syll_test/it-sample"
  )
