from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import os

mywd = os.path.dirname(os.path.abspath(__file__))

setup(
        name = 'collatz',
        cmdclass = {'build_ext': build_ext},
        packages = ['collatz'],
        package_dir = {'collatz': 'src'},
        ext_modules = [Extension("collatz._compute", ["src/_compute.pyx"], include_dirs=[os.path.join(mywd, 'local/include')], libraries=['mpir'], library_dirs=[os.path.join(mywd, 'local/lib')])]
    )
