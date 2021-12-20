# frequencySelctiveFading_OFDM-ADC-system

内容: 

params: H---信道估计
1. 频域上生成1024个pilot 
2. pilots做IFFT变为时域讯号
3. 加上GI 
4. 时域讯号上加fading和awgn计算
5. 去掉GI
6. 做FFT转为频域讯号
7. received_pilot[i]/pilot[i] = H[i]
8. 最后获得H[i]的共轭, 用于后续传输的OFDM讯号估计信道 Z[m]=R[m]*H^(*)[m] 


循环主体
1. QPSK-OVERSAMPLING-OFDM SYMBOL
2. 加上GI 
4. 时域讯号上加fading和awgn计算
5. 去掉GI
6. 做FFT转为频域讯号
7. Z[m]=R[m]*H^(*)[m] 
8. 计算BER

