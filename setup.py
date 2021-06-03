VERSION='0.1.dev'

from setuptools import setup

with open("README.md", "r") as doc:
    long_description = doc.read()
    doc.close()

with open("requirements.txt", "r") as mFile:
    requirements = mFile.read().split("\n")
    mFile.close()

require = [i for i in requirements if not i == ""]

setup(
    name="dsl",
    version=VERSION,
    author="Ajith Ramachandran",
    author_email="ajithar204@gmail.com",
    description="Data Structures for Python",
    long_description=long_description,
    long_description_content_type='text/markdown',
    url="https://github.com/AjithRamachandran/dsl",
    keywords='data structures',
    license='MIT',
    packages=['dsl'],
    install_requires=require,
    tests_require=['unittest'],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Programming Language :: Python :: Implementation :: CPython",
    ],
    python_requires='>=3.7',
)
