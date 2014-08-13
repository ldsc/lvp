CONFIG += warn_on \
          qt \
          thread \
          console
TEMPLATE = subdirs
TRANSLATIONS = lvp_en.ts \
               lvp_pt.ts
#CODECFORSRC = UTF-8
SUBDIRS += src
