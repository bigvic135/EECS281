struct city{
    int north , south;
};

bool compare(city first , city second){
    if (first.north == second.north){
        return first.south < second.south;
    }
    else{
        return first.north < second.north;
    }
}


int maxBridges(city values [] , int n){
    sort(values, values + n, compare);
    int lis[n];
    // initialize lis array as 1s, n runtime
    for(int i = 0; i < n; ++i){
        lis[i] = 1;
    }
    // fill in correct south coordinate lis, nˆ2 runtime
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if (values[i].south >= values[j].south && lis[i] < 1 + lis[j]){
                lis[i] = 1 + lis[j];
            }
        }
    }
    for(int i = 0; i < n; ++i){
        if(result < lis[i]){
            result = lis[i];
        }
    }
    return result;
}
