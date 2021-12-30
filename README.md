# frequencySelctiveFading_OFDM-ADC-system

内容:

params: H---信道估计

1. 频域上生成 1024 个 pilot
2. pilots 做 IFFT 变为时域讯号
3. 加上 GI
4. 时域讯号上加 fading 和 awgn 计算
5. ADCestimation
6. 去掉 GI
7. 做 FFT 转为频域讯号
8. filter 出 QPSK DATA
9. received_pilot[i]/pilot[i] = H[i]
10. 最后获得 H[i], 用于后续传输的 OFDM 讯号估计信道 Z[m]=R[m]/H[m]

## 中文乱码问题

默认为 ANSI, 部分为 UTF-8, 用 codeblocks 或 notepad 另存为即可
