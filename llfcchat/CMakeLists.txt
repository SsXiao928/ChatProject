cmake_minimum_required(VERSION 3.19)
project(llfcchat LANGUAGES CXX)

#使用网络模块要添加NetWork
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

#提升控件为自定义控件要添加，不然找不到文件
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

#添加资源文件（qrc样式文件）
# 添加资源文件
qt_add_resources(RESOURCE_SOURCES  ${CMAKE_SOURCE_DIR}/rc.qrc)

qt_add_executable(llfcchat
    WIN32 MACOSX_BUNDLE
    ${RESOURCE_SOURCES}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    adduseritem.cpp adduseritem.h adduseritem.ui
    applyfriend.cpp applyfriend.h applyfriend.ui
    bubbleframe.cpp bubbleframe.h
    charuserlist.cpp charuserlist.h
    chatdialog.cpp chatdialog.h chatdialog.ui
    chatitembase.cpp chatitembase.h
    chatpage.cpp chatpage.h chatpage.ui
    chatuserlist.cpp chatuserlist.h
    chatuserwid.cpp chatuserwid.h chatuserwid.ui
    chatview.cpp chatview.h
    clickedbtn.cpp clickedbtn.h
    clickedlabel.cpp clickedlabel.h
    clickedoncelabel.cpp clickedoncelabel.h
    customizeedit.cpp customizeedit.h
    findsuccessdlg.cpp findsuccessdlg.h findsuccessdlg.ui
    form.cpp form.h form.ui
    global.cpp global.h
    httpmgr.cpp httpmgr.h
    listitembase.cpp listitembase.h
    loadingdlg.cpp loadingdlg.h loadingdlg.ui
    logindialog.cpp logindialog.h logindialog.ui
    main.cpp mainwindow.cpp mainwindow.h mainwindow.ui
    messagetextedit.cpp messagetextedit.h
    picturebubble.cpp picturebubble.h
    registerdialog.cpp registerdialog.h registerdialog.ui
    resetdialog.cpp resetdialog.h resetdialog.ui
    searchlist.cpp searchlist.h
    singleton.h
    statewidget.cpp statewidget.h
    tcpmgr.cpp tcpmgr.h
    textbubble.cpp textbubble.h
    timerbtn.cpp timerbtn.h
    userdata.cpp userdata.h
    usermgr.cpp usermgr.h
    rc.qrc
    friendlabel.h friendlabel.cpp friendlabel.ui
    contactuserlist.h contactuserlist.cpp
    conuseritem.h conuseritem.cpp conuseritem.ui
    grouptipitem.h grouptipitem.cpp grouptipitem.ui
    applyfriendpage.h applyfriendpage.cpp applyfriendpage.ui
    applyfrienditem.h applyfrienditem.cpp applyfrienditem.ui
    applyfriendlist.h applyfriendlist.cpp
    findfaildlg.h findfaildlg.cpp findfaildlg.ui
    authenfriend.h authenfriend.cpp authenfriend.ui
)

target_link_libraries(llfcchat
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

include(GNUInstallDirs)

install(TARGETS llfcchat
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#拷贝文件到指定目录
configure_file(config.ini ${CMAKE_CURRENT_BINARY_DIR}/config.ini COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/static/ DESTINATION ${CMAKE_BINARY_DIR}/static)

qt_generate_deploy_app_script(
    TARGET llfcchat
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
