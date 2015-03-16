//
//  main.cpp
//  The BlackList 3
//
//  Created by Yunfan Jin on 3/14/15.
//  Copyright (c) 2015 yunfan.jin. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
using namespace std;

class Calculator{
    int N, K;
    vector<vector<long>> multi_list;
    
public:
    Calculator(int K, int N, const vector<vector<int>> &money):K(K),N(N){
        for (int i=0; i<K; i++) {
            vector<long> list;
            long sum = 0;
            for (int j=0; j<N; j++) {
                list.push_back(sum);
                sum += money[i][j];
            }
            list.push_back(sum);
            multi_list.push_back(list);
        }
    }
    long amount(int mercenary, int start, int end){
        return multi_list[mercenary][end] - multi_list[mercenary][start];
    }
};

inline bool isAvailable(int identifier, int mercenary){
    return (identifier | (1<<mercenary)) != identifier;
}

inline int combine(int identifier, int mercenary){
    return (identifier | (1<<mercenary));
}

void addValue(vector<map<int, long>> &dp, int level, int identifier, long cost){
    if (dp[level].find(identifier) == dp[level].end()) {
        dp[level][identifier] = cost;
    }else if(dp[level][identifier] > cost){
        dp[level][identifier] = cost;
    }
}

int main(int argc, const char * argv[]) {
    int N, K, tmp;
    cin >> N >> K;
    vector<vector<int>> money(K, vector<int>(N, 0));
    for (int i=0; i<K; i++) {
        for (int j=0; j<N; j++) {
            cin >> tmp;
            money[i][j] = tmp;
        }
    }
    Calculator cal(K, N, money);
    vector<map<int, long>> dp;
    dp.push_back(map<int, long>());
    for (int i=0; i<K; i++) {
        dp[0][1<<i] = cal.amount(i, 0, 1);
    }
    for (int level = 1; level < N; level ++) {
        dp.push_back(map<int, long>());
        for (int mercenary = 0; mercenary < K; mercenary ++) {
            dp[level][1<<mercenary] = cal.amount(mercenary, 0, level +1);
            for (int j=0; j<level; j++) {
                for (auto iter: dp[j]) {
                    int identifier = iter.first;
                    if (isAvailable(identifier, mercenary)) {
                        long cost = iter.second + cal.amount(mercenary, j+1, level+1);
                        addValue(dp, level, combine(identifier, mercenary), cost);
                    }
                }
            }
        }
    }
    
    auto iter = dp[N-1].begin();
    long min = iter->second;
    for (; iter != dp[N-1].end(); iter++) {
        if (min > iter->second) {
            min = iter->second;
        }
    }
    cout << min << endl;
    
    return 0;
}
