////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../Header_file/graphicsclass.h"


GraphicsClass::GraphicsClass()
	:m_D3D(0),
	m_Camera(0),
	m_Model(0),
	m_LightShader(0),
	m_LightFx(0),
	m_Light(0),
	m_Text(0),
	m_PlayerX(0),
	m_PlayerY(0)
{}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX BaseViewMatrix;

	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}


	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}


	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// ī�޶� ��ġ ���
	m_Camera->SetPosition(0.0f, 2.0f, -5.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(BaseViewMatrix);
	

	m_Text = new TextClass;
	if (!m_Text) {
		return false;
	}

	//render �Ҷ��� �ű�� �ȵǳ�?
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, BaseViewMatrix);
	if (!result)
	{
		return false;
	}

	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}


	// ��� �и� �Ұ�
	result = m_Model->Initialize(m_D3D->GetDevice(), L"Resource/test.obj", L"Resource/model.txt", L"Resource/uv_snap.dds", L"Resource/uv_snap2.dds"); //change file
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_LightFx = new LightFxClass;
	if (!m_LightFx)
	{
		return false;
	}

	result = m_LightFx->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light fx object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}




	SetMemberVariable();
	return true;
}
void GraphicsClass::SetMemberVariable() {

	// light object �� ����
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(0.5f, 0.5f, 0.5f, 0.5f);
	m_Light->SetDirection(-1.0f, 0.0f, 0.0f);
	m_Light->setSpecularColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	return;
}



void GraphicsClass::Shutdown()
{

	if (m_Text){
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}


	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}


	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_LightFx)
	{
		m_LightFx->Shutdown();
		delete m_LightFx;
		m_LightFx = 0;
	}


	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}


	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


//�� frame ���� ����Ǵ� �Լ�
bool GraphicsClass::Frame()
{
	bool result;

	// �׷��� �� �׸���
	// ���� ���⼭ ���� ���� �����ϴ°� ������?
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::SetHardWareData(MouseXY MousePosition, MouseXY PlayerPosition,int fps, int cpuPercent, float time)
{

	bool result;

	//typdef MousePosition
	m_mouseX = MousePosition.mouseX;
	m_mouseY = MousePosition.mouseY;

	

	m_PlayerX = PlayerPosition.mouseX;
	m_PlayerY = PlayerPosition.mouseY;

	//m_sentence1�� ���� ä���ִ� ���
	result = m_Text->SetMousePosition(MousePosition.mouseX, MousePosition.mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->setFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetCpu(cpuPercent, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX mSphereWorld[10];
	XMMATRIX mCylWorld[10];
	XMMATRIX mBoxWorld;
	XMMATRIX mGridWorld;
	XMMATRIX mCenterSphere;

	XMMATRIX mView;
	XMMATRIX mProj;

	XMMATRIX I = XMMatrixIdentity();
	mGridWorld = I;
	mView = I;
	mProj = I;

	XMMATRIX boxScale = XMMatrixScaling(2.0f, 1.0f, 2.0f);
	XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
	mBoxWorld = XMMatrixMultiply(boxScale, boxOffset);

	XMMATRIX centerSphereScale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	XMMATRIX centerSphereOffset = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	mCenterSphere = XMMatrixMultiply(centerSphereScale, centerSphereOffset);

	for (int i = 0; i < 5; ++i)
	{
		mCylWorld[i * 2 + 0] = XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f);
		mCylWorld[i * 2 + 1] = XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i * 5.0f);

		mSphereWorld[i * 2 + 0] = XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f);
		mSphereWorld[i * 2 + 1] = XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i * 5.0f);
	}

	ModelClass::InstanceData Boxdata;
	m_Model->GetBoxeData(Boxdata);
	ModelClass::InstanceData Griddata;
	m_Model->GetGrideData(Griddata);
	ModelClass::InstanceData Spheredata;
	m_Model->GetSphereeData(Spheredata);
	ModelClass::InstanceData Cylinderedata;
	m_Model->GetCylindereData(Cylinderedata);


	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMVECTOR Direction, DiffuseColor, AmbeintColor, SpecularColor, CameraPostion;

	float SpecularPower;
	bool result;

	float rotationX;
	float rotationY;

	rotationX = (float)XM_PI * (m_mouseX*0.0008f);
	rotationY = (float)XM_PI * (m_mouseY*0.0008f);

	
	// ���۸� ���� ���� ����
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// View matrix �� �����ϰ� ī�޶��� �⺻ ���� ����
	m_Camera->Render();

	// ���̴� ���࿡ �ʿ��� ���� ���������� ����
	m_Camera->GetViewMatrix(viewMatrix);
	//�̰� ���� ��� �����ִµ�?
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	CameraPostion = XMLoadFloat3(&m_Camera->GetPosition());

	

	// ���� ����� ������ ���������� ����
	m_Light->GetDirection(Direction);
	m_Light->GetDiffuseColor(DiffuseColor);
	m_Light->GetAmbientColor(AmbeintColor);
	m_Light->GetSpecularColor(SpecularColor);
	SpecularPower = m_Light->GetSpecularPower();


	

	// ��ü�� ȸ���� ����
	// ī�޶��� ȸ������ ��ü ����
	//���� RENDER() �� �ƴ϶� FRAME() �� �־ �Ǵ°� �ƴ�?
	//��ġ�� XMFLOAT �� �����ϸ� ���ݾ� ��û��
	viewMatrix = viewMatrix * XMMatrixTranslation(m_PlayerX, 0, m_PlayerY)*XMMatrixRotationY(-rotationX)*XMMatrixRotationX(-rotationY);
	//viewMatrix = viewMatrix * XMMatrixTranslation(-m_PlayerX, 0, -m_PlayerY)*XMMatrixRotationY(-rotationX)*XMMatrixRotationX(-rotationY);
	//���� ���� ���� ���� device context �� ������
	m_Model->Render(m_D3D->GetDeviceContext());




	// �־��� ������ fx���� ������ �� ����
	HR(result = m_LightFx->Render(m_D3D->GetDeviceContext(), Griddata.IndexCount, Griddata.IndexOffset, Griddata.VertexOffset, mGridWorld, viewMatrix, projectionMatrix, Direction, DiffuseColor, AmbeintColor, CameraPostion, SpecularColor, SpecularPower, m_Model->GetTexture()));
	HR(result = m_LightFx->Render(m_D3D->GetDeviceContext(), Boxdata.IndexCount, Boxdata.IndexOffset, Boxdata.VertexOffset, mBoxWorld, viewMatrix, projectionMatrix, Direction, DiffuseColor, AmbeintColor, CameraPostion, SpecularColor, SpecularPower, m_Model->GetTexture()));
	HR(result = m_LightFx->Render(m_D3D->GetDeviceContext(), Spheredata.IndexCount, Spheredata.IndexOffset, Spheredata.VertexOffset, mCenterSphere, viewMatrix, projectionMatrix, Direction, DiffuseColor, AmbeintColor, CameraPostion, SpecularColor, SpecularPower, m_Model->GetTexture()));

	for (int i = 0; i < 10; i++) {
		HR(result = m_LightFx->Render(m_D3D->GetDeviceContext(), Cylinderedata.IndexCount, Cylinderedata.IndexOffset, Cylinderedata.VertexOffset, mCylWorld[i], viewMatrix, projectionMatrix, Direction, DiffuseColor, AmbeintColor, CameraPostion, SpecularColor, SpecularPower, m_Model->GetTexture()));
	}

	for (int i = 0; i < 10; i++) {
		HR(result = m_LightFx->Render(m_D3D->GetDeviceContext(), Spheredata.IndexCount, Spheredata.IndexOffset, Spheredata.VertexOffset, mSphereWorld[i], viewMatrix, projectionMatrix, Direction, DiffuseColor, AmbeintColor, CameraPostion, SpecularColor, SpecularPower, m_Model->GetTexture()));
	}
	

	






	// ���ڵ� �Բ� ���ư��� �ʰ� �ϱ� ���� ���ο� worldmatrix ����
	// �и� �ʼ�
	m_D3D->GetWorldMatrix(worldMatrix);

	// 2d �������� ���� ���� �ѱ�
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	HR(m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix));

	// 2d �������� ���� ���� ����
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	// ���� �׸���
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::setValue() {

	


	return true;
}