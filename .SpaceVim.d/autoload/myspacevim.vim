" My custom settings

function! myspacevim#before() abort
  " Short cut to 'my_notes' indexing page: [SPC]+N
  call SpaceVim#custom#SPC('nnoremap', ['N'], 'e $NOTES_DIR/index.md <CR> cd $NOTES_DIR<CR>', 'Go to NOTES.', 1)
  
  " Neomake C/C++ config 
  let g:neomake_c_enabled_makers = ['gcc']
  let g:neomake_c_gcc_maker = { 'args': [ 
                                          \ '-Wall', 
                                          \ '-Wextra', 
                                          \ '-Weverything',
                                          \ '-pedantic'],
                              \ 'includes': [
                                          \ '-I/usr/include',
                                          \ '-I/home/dphan/ROOT/install/include',
                                          \ '-I/home/dphan/Geant4/install/include/Geant4']}
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
                                            \ '-I/home/dphan/Geant4/install/include/Geant4']}
  
  " YCM keybinds
  " For C++
  call SpaceVim#custom#LangSPCGroupName('cpp', ['y'], 'YouCompleteMe')
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'd'], 'YcmCompleter GoToDeclaration', 'Go to declaration.', 1)
  call SpaceVim#custom#LangSPC('cpp', 'nnoremap', ['y', 'D'], 'YcmCompleter GoToDefinition', 'Go to definition.', 1)
  " For C
  call SpaceVim#custom#LangSPCGroupName('c', ['y'], 'YouCompleteMe')
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'd'], 'YcmCompleter GoToDeclaration', 'Go to declaration.', 1)
  call SpaceVim#custom#LangSPC('c', 'nnoremap', ['y', 'D'], 'YcmCompleter GoToDefinition', 'Go to definition.', 1)
endfunction

function! myspacevim#after() abort
endfunction
