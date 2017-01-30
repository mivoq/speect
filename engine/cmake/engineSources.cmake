######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 25 March 2008                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                         Speect Engine source files                                 #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_FILES

######## src/base ##################
    
    # src/base
    src/base/base.c
  
    # src/base/containers
    src/base/containers/buffer/buffer.c
    src/base/containers/hashtable/hash_functions.c
    src/base/containers/hashtable/hash_table.c
    src/base/containers/list/list.c

    # src/base/errdbg
    src/base/errdbg/errdbg_impl.c
    src/base/errdbg/errdbg_utils.c

    # src/base/log
    src/base/log/event/event.c
    src/base/log/layout/layout.c
    src/base/log/layout/standard.c
    src/base/log/streams/console.c
    src/base/log/streams/file.c
    src/base/log/streams/stream.c
    src/base/log/logger.c

    # src/base/obj
    src/base/objsystem/class.c
    src/base/objsystem/object.c
    src/base/objsystem/objsystem.c
    src/base/objsystem/primitives.c

    # src/base/strings
    src/base/strings/char.c
    src/base/strings/regexp.c
    src/base/strings/sprint.c
    src/base/strings/strings.c
    src/base/strings/str_list.c

    # src/base/utils
    src/base/utils/alloc.c
    src/base/utils/byteswap.c
    src/base/utils/envvar.c
    src/base/utils/path.c
    src/base/utils/smath.c
    src/base/utils/stime.c
    src/base/utils/utils.c
    src/base/utils/vernum.c


######## src/containers ############

    # src/containers
    src/containers/container.c
    src/containers/containers.c
    src/containers/iterator.c

    # src/containers/list
    src/containers/list/containers_list.c
    src/containers/list/list.c
    src/containers/list/list_list.c
    src/containers/list/list_list_iterator.c
 
    # src/containers/map
    src/containers/map/containers_map.c
    src/containers/map/map.c   
    src/containers/map/map_hash_table.c
    src/containers/map/map_hash_table_iterator.c
    src/containers/map/map_list.c
    src/containers/map/map_list_iterator.c


######## src/datasources ###########

    # src/datasources
    src/datasources/data_reader.c
    src/datasources/data_source.c
    src/datasources/data_writer.c
    src/datasources/datasources.c  
    src/datasources/file_source.c  
    src/datasources/generic_source.c
    src/datasources/load_int.c
    src/datasources/load_real.c
    src/datasources/mmapfile_source.c  


######## src/hrg ###################

    # src/hrg
    src/hrg/hrg.c
    src/hrg/item.c
    src/hrg/item_content.c
    src/hrg/item_path.c 
    src/hrg/relation.c
    src/hrg/utterance.c

    # src/hrg/processors
    src/hrg/processors/featprocessor.c
    src/hrg/processors/processors.c
    src/hrg/processors/uttprocessor.c


######## src/main ###################

    # src/main
    src/main/loggers.c
    src/main/main.c
    src/main/managers.c
    src/main/modules.c
    src/main/plugin_path.c


######## src/pluginmanager #########

    # src/pluginmanager
    src/pluginmanager/dso.c
    src/pluginmanager/dynamic_loading.c
    src/pluginmanager/library.c
    src/pluginmanager/manager.c
    src/pluginmanager/pluginmanager.c
    src/pluginmanager/plugin_object.c


######## src/serialization #########

    # src/serialization/json
    src/serialization/json/json.c
    src/serialization/json/json_decode.c
    src/serialization/json/json_lex.c
    src/serialization/json/json_parse_config.c
    src/serialization/json/json_parser.c

    # src/serialization
    src/serialization/serialization.c
    src/serialization/serialize.c
    src/serialization/serialized_file.c
    src/serialization/serialized_object.c


######## src/utils #################

    # src/utils
    src/utils/token.c
    src/utils/tokenstream.c
    src/utils/tokenstream_file.c
    src/utils/tokenstream_string.c
    src/utils/utils.c


######## src/voicemanager ##########

    # src/voicemanager/loaders
    src/voicemanager/loaders/data_config.c
    src/voicemanager/loaders/feat_processor.c
    src/voicemanager/loaders/features.c
    src/voicemanager/loaders/info.c
    src/voicemanager/loaders/plugins.c
    src/voicemanager/loaders/utt_processor.c
    src/voicemanager/loaders/utt_types.c

    # src/voicemanager
    src/voicemanager/manager.c
    src/voicemanager/voice.c
    src/voicemanager/voicemanager.c
)
 

#------------------------------------------------------------------------------------#
#                         Speect Engine header files                                 #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_INCLUDE_FILES

######## src/base ##################
 
   # src/base
   src/base/base.h

   # src/base/containers
   src/base/containers/buffer/buffer.h
   src/base/containers/containers.h
   src/base/containers/hashtable/hash_functions.h
   src/base/containers/hashtable/hash_table.h
   src/base/containers/list/list.h

   # src/base/errdbg
   src/base/errdbg/errdbg_defs.h
   src/base/errdbg/errdbg.h
   src/base/errdbg/errdbg_impl.h
   src/base/errdbg/errdbg_macros.h
   src/base/errdbg/errdbg_utils.h

   # src/base/log
   src/base/log/event/event.h
   src/base/log/layout/layout.h
   src/base/log/layout/layout_impl.h
   src/base/log/layout/standard.h
   src/base/log/log.h
   src/base/log/streams/console.h
   src/base/log/streams/file.h
   src/base/log/streams/stream_impl.h
   src/base/log/streams/streams.h
   
   # src/base/obj
   src/base/objsystem/class.h
   src/base/objsystem/object_def.h
   src/base/objsystem/object.h
   src/base/objsystem/object_macros.h
   src/base/objsystem/objsystem.h
   src/base/objsystem/primitives.h

   # src/base/strings
   src/base/strings/char.h
   src/base/strings/regexp.h
   src/base/strings/sprint.h
   src/base/strings/strings.h
   src/base/strings/utf8.h
   src/base/strings/str_list.h

   # src/base/threads
   src/base/threads/threads.h
   
   # src/base/utils
   src/base/utils/alloc.h
   src/base/utils/byteswap.h
   src/base/utils/envvar.h
   src/base/utils/path.h
   src/base/utils/smath.h
   src/base/utils/stime.h
   src/base/utils/types.h
   src/base/utils/utils.h
   src/base/utils/vernum.h


######## src/containers ############
  
   # src/containers
   src/containers/container.h
   src/containers/containers.h
   src/containers/iterator.h

   # src/containers/list
   src/containers/list/containers_list.h
   src/containers/list/list.h
   src/containers/list/list_list.h
   src/containers/list/list_list_iterator.h

   # src/containers/map
   src/containers/map/containers_map.h
   src/containers/map/map.h
   src/containers/map/map_hash_table.h
   src/containers/map/map_hash_table_iterator.h
   src/containers/map/map_list.h
   src/containers/map/map_list_iterator.h


######## src/datasources ###########

   # src/datasources
   src/datasources/data_reader.h
   src/datasources/data_source.h
   src/datasources/datasources.h
   src/datasources/data_writer.h
   src/datasources/file_source.h
   src/datasources/generic_source.h
   src/datasources/load_int.h
   src/datasources/load_real.h
   src/datasources/mmapfile.h
   src/datasources/mmapfile_source.h

######## src/hrg ##################

   # src/hrg
   src/hrg/hrg_defs.h
   src/hrg/hrg.h
   src/hrg/item_content.h
   src/hrg/item.h
   src/hrg/item_path.h
   src/hrg/relation.h
   src/hrg/utterance.h

   # src/hrg/processors
   src/hrg/processors/featprocessor.h
   src/hrg/processors/processors.h
   src/hrg/processors/uttprocessor.h


######## src/include ###############
   
   # src/include
   src/include/common.h
   src/include/defs.h
   src/include/syslibs.h
   src/include/spctconfig.h
   src/include/version.h
   src/include/visibility.h


######## src/main ##################
 
   # src/main
   src/main/loggers.h
   src/main/main.h
   src/main/managers.h
   src/main/modules.h
   src/main/plugin_path.h


######## src/pluginmanager #########

   # src/pluginmanager
   src/pluginmanager/dso.h
   src/pluginmanager/dynamic_loading.h
   src/pluginmanager/library.h
   src/pluginmanager/manager.h
   src/pluginmanager/plugin.h
   src/pluginmanager/pluginmanager.h
   src/pluginmanager/plugin_object.h
 

######## src/serialization #########

   # src/serialization
   src/serialization/serialization.h
   src/serialization/serialized_file.h
   src/serialization/serialized_object.h
   src/serialization/serialize.h
   
   # src/serialization/json
   src/serialization/json/json_decode.h
   src/serialization/json/json.h
   src/serialization/json/json_lex.h
   src/serialization/json/json_parse_config.h
   src/serialization/json/json_parser.h


######## src (Main include) ########
   
   src/speect.h


######## src/utils #################

   # src/utils
   src/utils/token.h
   src/utils/tokenstream.h
   src/utils/tokenstream_file.h
   src/utils/tokenstream_string.h
   src/utils/utils.h
   

######## src/voicemanager ##########

   # src/voicemanager
   src/voicemanager/manager.h
   src/voicemanager/voice.h
   src/voicemanager/voicemanager.h

   # src/voicemanager/loaders
   src/voicemanager/loaders/data_config.h
   src/voicemanager/loaders/feat_processor.h
   src/voicemanager/loaders/features.h
   src/voicemanager/loaders/info.h
   src/voicemanager/loaders/loaders.h
   src/voicemanager/loaders/plugins.h
   src/voicemanager/loaders/utt_processor.h
   src/voicemanager/loaders/utt_types.h
)
