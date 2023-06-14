#Find image assets in icons folder and put it into qrc

#Copyright (c) 2022-2023 Oleksandr Movchan

#This software is provided 'as-is', without any express or implied
#warranty. In no event will the authors be held liable for any damages
#arising from the use of this software.

#Permission is granted to anyone to use this software for any purpose,
#including commercial applications, and to alter it and redistribute it
#freely, subject to the following restrictions:

#    1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.

#    2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.

#    3. This notice may not be removed or altered from any source
#    distribution.

function(GENERATE_ICONS_QRC directory outputFile)
    file(GLOB_RECURSE images ${directory}/*.svg ${directory}/*.png ${directory}/*.ico ${directory}/*.gif)
    list(LENGTH images listSize)

    message(STATUS "Assets found: " ${listSize})

    file(WRITE ${outputFile} "<RCC>\n    <qresource prefix=\"/icons\">\n")
    foreach(file ${images})
        string(LENGTH ${directory}/ directoryLength)
        string(SUBSTRING ${file} ${directoryLength} -1 fileName)
        file(APPEND ${outputFile} "        <file>${fileName}</file>\n")
    endforeach()
    file(APPEND ${outputFile} "    </qresource>\n</RCC>")

endfunction()
