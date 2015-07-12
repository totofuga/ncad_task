# ncad_task
# 概要
ネットワーク内に接続された不正なMACアドレスを持つNICを発見した場合
以下に指定する処理を行えます。
- mailで報告
- 即時排除
- 指定時間排除

# 使い方


## 設定

スクリプトを実行するディレクトリに
./macaddr.conf
許可するMACアドレスを:区切り１６進数で１行ずつ並べてください

例
```
AA::BB::CC::DD::EE::FF
00::11::22::33::44::55
```

## 実行
例
```
./ncad_task -m -f "fujita.y@gmail.com" -t "fujita.y@gamil.com" # mail送信
./ncad_task -e # 即時排除
./ncad_task -s # 10秒後に排除
```
- -m : メールを送信モード(default)
- -e : 即時排除モード
- -s sec : 指定秒後排除しますsecで秒数指定
- -i ifname : インターフェースnameを指定します(default eth0)
- -c config_path : ./macaadr.conf以外のコンフィグを使用する場合に指定します
- -f addr : メール送信元
- -f addr : メール送信先
