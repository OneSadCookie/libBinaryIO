require 'rbuild-cfamily'

sources = Dir['*.c']
sources.delete('test_main.c')
objects = build_objects(:sources => sources,
                        :extra_cflags => '-I. "-DBinIO_STDINT_HEADER=<stdint.h>"')

build_archive(:archive => 'libBinaryIO.a',
              :objects => objects)

test_objects = build_objects(:sources => ['test_main.c'],
                             :extra_cflags => '-I. "-DBinIO_STDINT_HEADER=<stdint.h>"')

build_executable(:executable => 'BinaryIOTest',
                 :objects => test_objects,
                 :archives => ['libBinaryIO.a'])
