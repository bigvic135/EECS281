void mapOutputProcess){
    for(size_t i = 0; i < numColors + 1; ++i){
          for(size_t j = 0; j < height; ++j){
                 for(size_t k = 0; k < width; ++k){
                       if(map3D[i][j][k] != '+' && map3D[i][j][k] != '@' && map3D[i][j][k] != '%'){
                            if(toupper(getColorChar(i)) == map2D[j][k] && isDoor(map2D[j][k])){//replace doors of same color with dots
                                  map3D[i][j][k] = '.';
                               }
                        else if(map2D[j][k] == '^' && getColorChar(i) == '^'){
                                    map3D[i][j][k] = '.';
                               }
                        else if(getColorChar(i) != '^' && map2D[j][k] == '@'){
                                     map3D[i][j][k] = '.';
                        }
                        else if(map2D[j][k] == getColorChar(i)){//get rid of buttons
                                     map3D[i][j][k] = '.';
                        }
                        else if(map3D[i][j][k] == '-' || map3D[i][j][k] == 'N' || map3D[i][j][k] == 'S' || map3D[i][j][k] == 'E' || map3D[i][j][k] == 'W' || map3D[i][j][k] == 'S'){
                                                                map3D[i][j][k] = map2D[j][k];
                         }
                       }
                            }
                    }
            }
}
