project('cpplox', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

sources = [
        'src/main.cpp',
        'src/interpreter/interpreter.cpp',
        'src/interpreter/scanner.cpp',
        'src/interpreter/expression.cpp',
]
cpplox = executable('cpplox', sources, install : true)

test('test', cpplox)