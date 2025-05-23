/**
* @param MCプロトコルを設定する際の定数をまとめたデータクラスです。
*/
class MCprotocolConfig {
    public:
        // 頭文字がDのデバイスコードの最大読取ワード数
        static constexpr int D_WORD_MAX_READ_SIZE = 4;

        // 頭文字がMのデバイスコードの最大読取点数(bit読み取り時)
        static constexpr int M_X_Y_S_T_C_BIT_MAX_READ_SIZE = 12; 
};