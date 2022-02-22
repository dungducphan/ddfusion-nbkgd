" My custom settings

function! myspacevim#before() abort
  " Neomake C/C++ config 
  let g:neomake_c_enabled_makers = ['gcc']
  let g:neomake_c_gcc_maker = { 'args': [ 
                                          \ '-Wall', 
                                          \ '-Wextra', 
                                          \ '-Weverything',
                                          \ '-pedantic'],
                              \ 'includes': [
                                          \ '-I/usr/include',
                                          \ '-I/home/dphan/ROOT/install_dev/include',
                                          \ '-I/home/dphan/ROOT/install/include',
                                          \ '-I/home/dphan/Geant4/install-11.00-release/include/Geant4']}
  let g:neomake_cpp_enabled_makers = ['gcc']
  let g:neomake_cpp_gcc_maker =   { 'args': [
                                            \ '-Wall',
                                            \ '-Wextra',
                                            \ '-Weverything',
                                            \ '-pedantic',
                                            \ '-Wno-sign-conversion'],
                                  \ 'includes': [
                                            \ '-I/usr/include',                  
                                            \ '-I/home/dphan/ROOT/install/include',
                                            \ '-I/home/dphan/ROOT/install_dev/include',
                                            \ '-I/home/dphan/ROOT/install/include/Math',
                                            \ '-I/home/dphan/Geant4/install-11.00-release/include/Geant4']}
  
  " YCM keybinds
  " For C++
  call SpaceVim#custom#LangSPCGroupName('cpp', ['y'], 'YouCompleteMe')
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'e'], 'YcmCompleter GoToDeclaration', 'Go to declaration.', 1)
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'd'], 'YcmCompleter GoToDefinition', 'Go to definition.', 1)
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'i'], 'YcmCompleter GoToInclude', 'Go to include file.', 1)
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'g'], 'YcmCompleter GoTo', 'Go to sensible destination.', 1)
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'm'], 'YcmCompleter GetDoc', 'Go to sensible destination.', 1)
  " For C
  call SpaceVim#custom#LangSPCGroupName('c', ['y'], 'YouCompleteMe')
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'e'], 'YcmCompleter GoToDeclaration', 'Go to declaration.', 1)
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'd'], 'YcmCompleter GoToDefinition', 'Go to definition.', 1)
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'i'], 'YcmCompleter GoToInclude', 'Go to include file.', 1)
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'g'], 'YcmCompleter GoTo', 'Go to sensible destination.', 1)
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'm'], 'YcmCompleter GetDoc', 'Go to sensible destination.', 1)
endfunction

function! myspacevim#after() abort
endfunction
