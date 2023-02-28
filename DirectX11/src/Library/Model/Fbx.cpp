#include "Fbx.h"

#include "../Shader/ShaderManager.h"
#include "../DirectX/Matrix.h"
#include "../DirectX/DirectXGraphics.h"

namespace engine
{
	Fbx::Fbx(FbxManager* fbx_manager_)
		: m_fbxManager(fbx_manager_)
	{
	}

	Fbx::~Fbx()
	{
		if (m_fbxImporter != nullptr)
		{
			m_fbxImporter->Destroy();
		}

		if (m_fbxScene != nullptr)
		{
			m_fbxScene->Destroy();
		}
	}

	bool Fbx::Load(const std::string& file_name_)
	{
		m_fbxScene = FbxScene::Create(m_fbxManager, "file_name_");
		if (!m_fbxScene) { return false; }

		m_fbxImporter = FbxImporter::Create(m_fbxManager, "file_name_");
		if (!m_fbxImporter)
		{
			m_fbxScene->Destroy();
			return false;
		}

		bool result{ true };

		if (!m_fbxImporter->Initialize(file_name_.c_str())) { result = false; }

		if (!m_fbxImporter->Import(m_fbxScene)) { result = false; }

		if (!result)
		{
			m_fbxImporter->Destroy();
			m_fbxScene->Destroy();

			return false;
		}

		FbxGeometryConverter converter(m_fbxManager);
		// ポリゴンを三角形にする
		converter.Triangulate(m_fbxScene, true);

		m_rootNode = m_fbxScene->GetRootNode();
		SearchNode(m_rootNode, FbxNodeAttribute::EType::eMesh, m_meshNodeList);
		SearchNode(m_rootNode, FbxNodeAttribute::EType::eSkeleton, m_skeletonNodeList);

		for (auto mesh_node_ : m_meshNodeList)
		{
			FbxMesh* mesh{ mesh_node_.second->GetMesh() };

			int skinNum = mesh->GetDeformerCount(FbxDeformer::eSkin);
			for (int i = 0; i < skinNum; i++)
			{
				// スキンを取得
				FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(i, FbxDeformer::eSkin);

				// クラスターの数を取得
				int clusterNum = skin->GetClusterCount();
				for (int j = 0; j < clusterNum; j++)
				{
					// クラスタを取得
					FbxCluster* cluster = skin->GetCluster(j);

					int pointNum = cluster->GetControlPointIndicesCount();
					int* pointAry = cluster->GetControlPointIndices();
					double* weightAry = cluster->GetControlPointWeights();

					WeightData weight{};

					for (int i = 0; i < pointNum; ++i)
					{
						// 頂点インデックスとウェイトを保存
						weight[pointAry[i]] = (float)weightAry[i];
					}

					m_boneWeightList[cluster->GetName()] = weight;

					// 初期姿勢
					FbxAMatrix initMat;
					cluster->GetTransformLinkMatrix(initMat);
					m_initMatrixList[cluster->GetName()] = initMat;
				}
			}
		}

		int animationNum = m_fbxImporter->GetAnimStackCount();
		m_canAnimation = animationNum;
		//for (int i = 0; i < animationNum; i++)
		//{
		//	FbxTakeInfo* fbxTakeInfo = m_fbxImporter->GetTakeInfo(i);
		//
		//	int end = fbxTakeInfo->mLocalTimeSpan.GetStop().Get();
		//}

		for (auto node : m_meshNodeList)
		{
			FbxMesh* mesh{ node.second->GetMesh() };

			// インデックスバッファ
			for (int i = 0; i < mesh->GetPolygonCount(); i++)
			{
				m_index[node.first].push_back(i * 3 + 2);
				m_index[node.first].push_back(i * 3 + 1);
				m_index[node.first].push_back(i * 3);
			}

			// 頂点バッファの取得
			FbxVector4* vertices = mesh->GetControlPoints();
			// インデックスバッファの取得
			int* indices = mesh->GetPolygonVertices();

			for (int i = 0; i < mesh->GetPolygonVertexCount(); i++)
			{
				CustomVertex customVertex{};
				int index = indices[i];

				// 頂点座標リストから座標を取得
				customVertex.vecesData[0] = -vertices[index][0];
				customVertex.vecesData[1] = vertices[index][1];
				customVertex.vecesData[2] = vertices[index][2];

				// 追加
				m_customVertexList[node.first].push_back(customVertex);
			}

			FbxArray<FbxVector4> normals;
			mesh->GetPolygonVertexNormals(normals);
			for (int i = 0; i < normals.Size(); i++)
			{
				m_customVertexList[node.first][i].nomalData[0] = -normals[i][0];
				m_customVertexList[node.first][i].nomalData[1] = normals[i][1];
				m_customVertexList[node.first][i].nomalData[2] = normals[i][2];
			}
		}

		for (auto customVertex : m_customVertexList)
		{
			D3D11_BUFFER_DESC vertexBufferDesc = {
				sizeof(CustomVertex) * (UINT)customVertex.second.size(),
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER,
				0,
				0,
				0 };

			D3D11_SUBRESOURCE_DATA vertexSubresData = {
				&customVertex.second[0],
				0,
				0 };

			if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresData, &m_vertexBufferList[customVertex.first])))
			{
				return false;
			}
		}

		for (auto index : m_index)
		{
			D3D11_BUFFER_DESC indexBufferDesc = {
				sizeof(UINT) * (UINT)index.second.size(),
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_INDEX_BUFFER,
				0,
				0,
				0 };

			D3D11_SUBRESOURCE_DATA indexSubresData = {
				&index.second[0],
				0,
				0 };

			if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresData, &m_indexBufferList[index.first])))
			{
				return false;
			}
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			ShaderManager::GetInstance()->GetShaderData(m_vShaderName),
			ShaderManager::GetInstance()->GetShaderSize(m_vShaderName),
			&m_inputLayout)))
		{
			return false;
		}

		D3D11_BUFFER_DESC constantBufferDesc = {
			sizeof(ConstantBuffer),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0,
			0
		};

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer)))
		{
			return false;
		}

		return true;
	}

	void Fbx::Render(const Vec3f& pos_, const Vec3f& rote_, const Vec3f& scale_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		DirectX::XMMATRIX world = Matrix::GetInstance()->CreateWorldMatrix(pos_, rote_, scale_);
		DirectX::XMMATRIX view = Matrix::GetInstance()->CreateViewMatrix();
		DirectX::XMMATRIX proj = Matrix::GetInstance()->CreateProjMatrix();

		ConstantBuffer constantBuffer;
		DirectX::XMStoreFloat4x4(&constantBuffer.world, DirectX::XMMatrixTranspose(world));
		DirectX::XMStoreFloat4x4(&constantBuffer.view, DirectX::XMMatrixTranspose(view));
		DirectX::XMStoreFloat4x4(&constantBuffer.proj, DirectX::XMMatrixTranspose(proj));

		DirectX::XMVECTOR light = Matrix::GetInstance()->CreateLightPosMatrix();
		DirectX::XMStoreFloat4(&constantBuffer.LightVec, light);
		constantBuffer.LightCol = Matrix::GetInstance()->CreateLightColMatrix();

		constantBuffer.col = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

		for (auto index : m_index)
		{
			context->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

			// 透過の設定
			DirectXGraphics::GetInstance()->SetUpBlendState();

			// GPUへ送るデータの設定
			DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);

			UINT strides = sizeof(CustomVertex);
			UINT offsets = 0;
			context->IASetInputLayout(m_inputLayout);
			context->IASetIndexBuffer(m_indexBufferList.at(index.first), DXGI_FORMAT_R32_UINT, 0);

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			context->IASetVertexBuffers(0, 1, &m_vertexBufferList.at(index.first), &strides, &offsets);
			context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
			context->PSSetConstantBuffers(0, 1, &m_constantBuffer);

			context->DrawIndexed(index.second.size(), 0, 0);
		}
	}


	void Fbx::RenderAnimation(const Vec3f& pos_, const Vec3f& rote_, const Vec3f& scale_, int frame_)
	{
		if (!m_canAnimation) { return; }

		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		DirectX::XMMATRIX world = Matrix::GetInstance()->CreateWorldMatrix(pos_, rote_, scale_);
		DirectX::XMMATRIX view = Matrix::GetInstance()->CreateViewMatrix();
		DirectX::XMMATRIX proj = Matrix::GetInstance()->CreateProjMatrix();

		ConstantBuffer constantBuffer;
		DirectX::XMStoreFloat4x4(&constantBuffer.world, DirectX::XMMatrixTranspose(world));
		DirectX::XMStoreFloat4x4(&constantBuffer.view, DirectX::XMMatrixTranspose(view));
		DirectX::XMStoreFloat4x4(&constantBuffer.proj, DirectX::XMMatrixTranspose(proj));

		DirectX::XMVECTOR light = Matrix::GetInstance()->CreateLightPosMatrix();
		DirectX::XMStoreFloat4(&constantBuffer.LightVec, light);
		constantBuffer.LightCol = Matrix::GetInstance()->CreateLightColMatrix();

		constantBuffer.col = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

		//for (auto meshNode : m_meshNodeList)
		for (auto index : m_index)
		{
			context->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

			// 透過の設定
			DirectXGraphics::GetInstance()->SetUpBlendState();

			// GPUへ送るデータの設定
			DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);

			UINT strides = sizeof(CustomVertex);
			UINT offsets = 0;
			context->IASetInputLayout(m_inputLayout);
			context->IASetIndexBuffer(m_indexBufferList.at(index.first), DXGI_FORMAT_R32_UINT, 0);

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//FbxLongLong frameTime = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
			FbxTime time;
			time.SetTime(0, 0, 0, 1, 0, FbxTime::eFrames60);

			FbxMatrix globalPosition = m_rootNode->EvaluateGlobalTransform(time * frame_);
			FbxVector4 t0 = m_rootNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 r0 = m_rootNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 s0 = m_rootNode->GetGeometricScaling(FbxNode::eSourcePivot);
			FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

			std::vector<CustomVertex> customVertex{ m_customVertexList[index.first] };

			//FbxMesh* mesh = meshNode.second->GetMesh();
			FbxMesh* mesh = m_meshNodeList[index.first]->GetMesh();

			FbxMatrix* clusterDeformation = new FbxMatrix[mesh->GetControlPointsCount()];
			memset(clusterDeformation, 0, sizeof(FbxMatrix) * mesh->GetControlPointsCount());

			FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
			int clusterNum = skin->GetClusterCount();
			for (int i = 0; i < clusterNum; i++)
			{
				FbxCluster* cluster = skin->GetCluster(i);
				FbxMatrix vertexTransformMatrix;
				FbxAMatrix referenceGlobalInitPosition;
				FbxAMatrix clusterGlobalInitPosition;
				FbxMatrix clusterGlobalCurrentPosition;
				FbxMatrix clusterRelativeInitPosition;
				FbxMatrix clusterRelativeCurrentPositionInverse;
				cluster->GetTransformMatrix(referenceGlobalInitPosition);
				referenceGlobalInitPosition *= geometryOffset;
				cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
				clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(time * frame_);
				clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
				clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
				vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

				for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++)
				{
					int index = cluster->GetControlPointIndices()[i];
					double weight = cluster->GetControlPointWeights()[i];
					FbxMatrix influence = vertexTransformMatrix * weight;
					clusterDeformation[index] += influence;
				}
			}

			int count = mesh->GetControlPointsCount();
			for (int i = 0; i < count; i++) {
				FbxVector4 outVertex = clusterDeformation[i].MultNormalize(mesh->GetControlPointAt(i));
				float x = (FLOAT)outVertex[0];
				float y = (FLOAT)outVertex[1];
				float z = (FLOAT)outVertex[2];

				customVertex[i].vecesData[0] = x;
				customVertex[i].vecesData[1] = y;
				customVertex[i].vecesData[2] = z;
			}

			delete[] clusterDeformation;

			ID3D11Buffer* vertexBuffer{};

			D3D11_BUFFER_DESC vertexBufferDesc = {
				sizeof(CustomVertex) * (UINT)customVertex.size(),
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER,
				0,
				0,
				0 };

			D3D11_SUBRESOURCE_DATA vertexSubresData = {
				&customVertex[0],
				0,
				0 };

			if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresData, &vertexBuffer)))
			{
				return;
			}

			context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
			context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
			context->PSSetConstantBuffers(0, 1, &m_constantBuffer);

			context->DrawIndexed(index.second.size(), 0, 0);

			vertexBuffer->Release();
		}
	}

	void Fbx::SearchNode(FbxNode* node_, FbxNodeAttribute::EType node_type_, std::map<std::string, FbxNode*>& node_list_)
	{
		for (int i = 0; i < node_->GetNodeAttributeCount(); i++)
		{
			FbxNodeAttribute* attribute{ node_->GetNodeAttributeByIndex(i) };

			if (attribute->GetAttributeType() == node_type_)
			{
				node_list_[node_->GetName()] = node_;
				break;
			}
		}

		for (int i = 0; i < node_->GetChildCount(); i++)
		{
			SearchNode(node_->GetChild(i), node_type_, node_list_);
		}
	}
}