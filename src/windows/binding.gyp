{
  "targets": [
    {
      "target_name": "hikopenapi",
      "sources": [ "hikopenapi.cpp" ],
      "include_dirs": ["./headers","./"],
      "dependencies": [],
      "library_dirs": [ './lib',"./"],
      'defines': [],
      'conditions' : [          
          ['OS=="linux"',{
            'sources': [],
            'defines':[]
          }],
          ['OS=="win"', {
            'sources':[],
            'defines':[]
          }]
      ],
      "cflags!": ["-fno-exceptions -Wl,-z,origin -Wl,-rpath='$ORIGIN/test'"],
      "cflags_cc!": ["-fno-exceptions -Wl,-z,origin -Wl,-rpath='$ORIGIN/test'"],
      "defines": ["NAPI_CPP_EXCEPTIONS"],

    },
     {
      "target_name": "copy_binary",
      "type":"none",
      "dependencies" : [ "hikopenapi" ],
      "copies": [
        {
          'destination': '<(module_root_dir)/../../lib/windows',
          'files': ['<(module_root_dir)/build/Release/hikopenapi.node']
        }
      ],
    }
  ]
}