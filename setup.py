from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension("Collatz", ["Collatz.pyx"], include_dirs=['/usr/local/include'], libraries=['mpir'], library_dirs=['/usr/local/lib'])]
)