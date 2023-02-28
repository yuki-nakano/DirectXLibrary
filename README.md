# DirectXLibrary
DirectX11でのライブラリ作成とそれを用いたゲーム作成　

DirectX11/src/Libraryファイルの中に入っています。
|ファイルパス|コード概要|
|--|--|
|DirectX|DirextX関連|
|HLSL|HLSL|
|Lib|外部ライブラリー|
|Model|Obj解析、描画|
|Shader|シェーダー生成、管理|
|Utility|Singleton|
|WindowsAPI|Window生成、キーボード、マウス|
|cso|コンパイル後HLSL|

---

FBXの描画にFBXSDKを使用しています。
プロジェクト内のFBXSDKフォルダ内にFbxSdkのincludeファイルとx64ファイルの追加をお願いします。  

DirectX11  
    ┣FBXSDK  
    ┃   ┣ include  
    ┃   ┗ x64  
    ┃   &emsp;    ┣ debug  
    ┃   &emsp;    ┗ relese  
    ┣ソリューション  

[FBXSDK](https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-3)からダウンロードお願いします。  
バージョンはFBX SDK 2020.3.2 VS2019 です。