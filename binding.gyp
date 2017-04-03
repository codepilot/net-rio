{ 
  "targets": [ 
    { 
      "target_name": "binding",
      "sources": [
        "./cpp/dllmain.cpp",
        "./cpp/dllmain.h",

				"./cpp/rio.cpp",
				"./cpp/rio.h",

        "./cpp/helper.h",

        "./cpp/EternalStrings.h",
				
				"index.js"
				

      ],

      "library_dirs": ["lib"],
      "libraries": [],
      "include_dirs": [ "include", "h" ],
      "defines": [ "_WINDLL", "UNICODE", "_UNICODE", "WIN32_LEAN_AND_MEAN" ]
    }
  ]
}