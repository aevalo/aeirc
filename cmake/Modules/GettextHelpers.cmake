FUNCTION(add_translation_creation target_name target_comment default_domain output_dir keywordspec locale)
  file(MAKE_DIRECTORY "${CMAKE_HOME_DIRECTORY}/${output_dir}/${locale}")
  set(pot_file "${default_domain}.pot")
  set(po_file "${output_dir}/${locale}/${default_domain}.po")
  file(TO_NATIVE_PATH ${po_file} native_po_file)
  file(TO_NATIVE_PATH "${output_dir}" native_output_dir)
  file(TO_NATIVE_PATH "${native_output_dir}/${pot_file}" native_pot_file)
  foreach(input_file ${ARGN})
    file(TO_NATIVE_PATH ${input_file} native_input_file)
    set(input_files ${input_files} ${native_input_file})
  endforeach(input_file)
  add_custom_target(${target_name} xgettext --default-domain=${default_domain} --output-dir=${native_output_dir} --output=${pot_file} --keyword=${keywordspec} --sort-output ${input_files}
                    COMMAND msginit --input=${native_pot_file} --output=${native_po_file} --locale=${locale}
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
                    COMMENT ${target_comment})
ENDFUNCTION(add_translation_creation)

FUNCTION(add_translation_update target_name target_comment default_domain output_dir keywordspec locale)
  file(MAKE_DIRECTORY "${CMAKE_HOME_DIRECTORY}/${output_dir}/${locale}")
  set(pot_file "${default_domain}.pot")
  set(po_file "${output_dir}/${locale}/${default_domain}.po")
  file(TO_NATIVE_PATH ${po_file} native_po_file)
  file(TO_NATIVE_PATH "${output_dir}" native_output_dir)
  file(TO_NATIVE_PATH "${native_output_dir}/${pot_file}" native_pot_file)
  foreach(input_file ${ARGN})
    file(TO_NATIVE_PATH ${input_file} native_input_file)
    set(input_files ${input_files} ${native_input_file})
  endforeach(input_file)
  add_custom_target(${target_name} xgettext --default-domain=${default_domain} --output-dir=${native_output_dir} --output=${pot_file} --keyword=${keywordspec} --sort-output ${input_files}
                    COMMAND msgmerge --update --sort-output --backup=existing --suffix=backup ${native_po_file} ${native_pot_file}
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
                    COMMENT ${target_comment} VERBATIM)
ENDFUNCTION(add_translation_update)

FUNCTION(add_translation_compile target_name target_comment default_domain locale)
  file(MAKE_DIRECTORY "${CMAKE_HOME_DIRECTORY}/locale/${locale}/LC_MESSAGES")
  set(mo_file "locale/${locale}/LC_MESSAGES/${default_domain}.mo")
  file(TO_NATIVE_PATH ${mo_file} native_mo_file)
  foreach(input_file ${ARGN})
    file(TO_NATIVE_PATH ${input_file} native_input_file)
    set(input_files ${input_files} ${native_input_file})
  endforeach(input_file)
  add_custom_target(${target_name} msgfmt --check --verbose --output-file=${native_mo_file} ${input_files}
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
                    COMMENT ${target_comment} VERBATIM)
ENDFUNCTION(add_translation_compile)

FUNCTION(create_translantion_targets _SOURCES)
  IF(NOT DEFINED GETTEXT_PO_BASE_DIR)
    SET(GETTEXT_PO_DIRECTORY ${PROJECT_BINARY_DIR}/po)
  ELSE(NOT DEFINED GETTEXT_PO_BASE_DIR)
    SET(GETTEXT_PO_DIRECTORY ${GETTEXT_PO_BASE_DIR}/po)
  ENDIF(NOT DEFINED GETTEXT_PO_BASE_DIR)
  IF(NOT DEFINED GETTEXT_MO_BASE_DIR)
    SET(GETTEXT_MO_DIRECTORY ${PROJECT_BINARY_DIR}/share/locale)
  ELSE(NOT DEFINED GETTEXT_MO_BASE_DIR)
    SET(GETTEXT_MO_DIRECTORY ${GETTEXT_MO_BASE_DIR}/share/locale)
  ENDIF(NOT DEFINED GETTEXT_MO_BASE_DIR)
  FILE(MAKE_DIRECTORY ${GETTEXT_PO_DIRECTORY} ${GETTEXT_MO_DIRECTORY})
  
  SET(GETTEXT_POT_FILE ${GETTEXT_PO_DIRECTORY}/${PROJECT_NAME}.pot)
  FILE(TO_NATIVE_PATH ${GETTEXT_PO_DIRECTORY}/${PROJECT_NAME}.pot _NATIVE_POT_FILE)
  
  FOREACH(SOURCE_FILE ${_SOURCES})
    FILE(TO_NATIVE_PATH ${SOURCE_FILE} NATIVE_SOURCE_FILE)
    LIST(APPEND _NATIVE_INPUT_FILES ${NATIVE_SOURCE_FILE})
  ENDFOREACH(SOURCE_FILE)
  
  FOREACH(LOCALE ${ARGN})
    FILE(MAKE_DIRECTORY ${GETTEXT_PO_DIRECTORY}/${LOCALE} ${GETTEXT_MO_DIRECTORY}/${LOCALE}/LC_MESSAGES)
    FILE(TO_NATIVE_PATH ${GETTEXT_PO_DIRECTORY}/${LOCALE}/${PROJECT_NAME}.po _NATIVE_PO_FILE)

    ADD_CUSTOM_TARGET(createpo_${PROJECT_NAME}_${LOCALE} xgettext --default-domain=${PROJECT_NAME} --output-dir=${GETTEXT_PO_DIRECTORY} --output=${_NATIVE_POT_FILE} --keyword=_ --sort-output ${_NATIVE_INPUT_FILES}
                    COMMAND msginit --input=${_NATIVE_POT_FILE} --output=${_NATIVE_PO_FILE} --locale=${LOCALE}
                    COMMENT "Creating translation for locale ${LOCALE}" VERBATIM)
    ADD_CUSTOM_TARGET(updatepo_${PROJECT_NAME}_${LOCALE} xgettext --default-domain=${PROJECT_NAME} --output-dir=${GETTEXT_PO_DIRECTORY} --output=${_NATIVE_POT_FILE} --keyword=_ --sort-output ${_NATIVE_SOURCE_FILES}
                    COMMAND msgmerge --update --sort-output --backup=existing --suffix=backup ${_NATIVE_POT_FILE} ${_NATIVE_PO_FILE}
                    COMMENT "Updating translation for locale ${LOCALE}" VERBATIM)
    ADD_CUSTOM_TARGET(makemo_${PROJECT_NAME}_${LOCALE} msgfmt --check --verbose --output-file=${GETTEXT_MO_DIRECTORY}/${LOCALE}/LC_MESSAGES/${PROJECT_NAME}.mo ${GETTEXT_PO_DIRECTORY}/${LOCALE}/${PROJECT_NAME}.po
                    COMMENT "Compiling translation for locale ${LOCALE}" VERBATIM)
                    
    LIST(APPEND GETTEXT_TARGETS_CREATE_PO create_po_${LOCALE})
    LIST(APPEND GETTEXT_TARGETS_UPDATE_PO update_po_${LOCALE})
    LIST(APPEND GETTEXT_TARGETS_MAKE_MO make_mo_${LOCALE})
  ENDFOREACH(LOCALE)
  
  ADD_CUSTOM_TARGET(createpo_${PROJECT_NAME} DEPENDS ${GETTEXT_TARGETS_CREATE_PO} COMMENT "Translations created!" VERBATIM)
  ADD_CUSTOM_TARGET(updatepo_${PROJECT_NAME} DEPENDS ${GETTEXT_TARGETS_UPDATE_PO} COMMENT "Translations updated!" VERBATIM)
  ADD_CUSTOM_TARGET(makemo_${PROJECT_NAME} DEPENDS ${GETTEXT_TARGETS_MAKE_MO} COMMENT "Translations compiled!" VERBATIM)
ENDFUNCTION(create_translantion_targets)
