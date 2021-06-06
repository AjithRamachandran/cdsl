VERSION='0.1.dev0'

from setuptools import setup, Extension

sources=[ 'cdsl/cdsl.c']

with open("README.md", "r") as doc:
    long_description = doc.read()
    doc.close()

with open("requirements.txt", "r") as mFile:
    requirements = mFile.read().split("\n")
    mFile.close()

require = [i for i in requirements if not i == ""]

cdsl = Extension('cdsl', sources=sources, include_dirs=['cdsl/include'])

setup(
    name="cdsl",
    version=VERSION,
    author="Ajith Ramachandran",
    author_email="ajithar204@gmail.com",
    description="Common Data Structures for Python",
    long_description=long_description,
    long_description_content_type='text/markdown',
    url="https://github.com/AjithRamachandran/cdsl",
    keywords='data structures',
    license='MIT',
    packages=['cdsl'],
    install_requires=require,
    tests_require=['unittest'],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Programming Language :: Python :: Implementation :: CPython",
    ],
    ext_modules=[cdsl],
    python_requires='>=3.7',
)