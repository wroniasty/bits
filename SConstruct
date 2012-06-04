import sys, os

env = Environment()
conf = Configure(env)

env['CPPDEFINES'] = []
if sys.byteorder == "little":
   env['CPPDEFINES'].append(['BITS_LITTLE_ENDIAN'])

env['CPPFLAGS'] = []
env['CPPFLAGS'].append ( '-I' + os.getcwd() + '/src' )

env.SConscript ('src/SConscript', variant_dir='build', exports=['env', 'conf'])
env.SConscript ('test/SConscript', exports=['env', 'conf'])

