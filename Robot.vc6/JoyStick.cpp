// JoyStick.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "JoyStick.h"

#include <windows.h>
#include <basetsd.h>
#include <dinput.h>
#include "resource.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTINPUT8       g_pDI              = NULL;         
LPDIRECTINPUTDEVICE8 g_pJoystick        = NULL;  
    DIJOYSTATE2 js;

BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoyStick dialog


CJoyStick::CJoyStick(CWnd* pParent /*=NULL*/)
	: CDialog(CJoyStick::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJoyStick)
	m_X1 = 0;
	m_X2 = 327;
	m_Y1 = -327;
	m_Y2 = -327;
	m_IO[0]=m_IO[1]=m_IO[2]=m_IO[3]=m_IO[4]=m_IO[5]=m_IO[6]=m_IO[7]=m_IO[8]=m_IO[9]=0;
	//}}AFX_DATA_INIT
}


void CJoyStick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJoyStick)
	DDX_Text(pDX, IDC_LX, m_X1);
	DDX_Text(pDX, IDC_LX2, m_X2);
	DDX_Text(pDX, IDC_LY1, m_Y1);
	DDX_Text(pDX, IDC_LY2, m_Y2);
	DDX_Text(pDX, IDC_EDIT2, m_IO[0]);
	DDX_Text(pDX, IDC_EDIT6, m_IO[1]);
	DDX_Text(pDX, IDC_EDIT7, m_IO[2]);
	DDX_Text(pDX, IDC_EDIT8, m_IO[3]);
	DDX_Text(pDX, IDC_EDIT9, m_IO[4]);
	DDX_Text(pDX, IDC_EDIT10, m_IO[5]);
	DDX_Text(pDX, IDC_EDIT11, m_IO[6]);
	DDX_Text(pDX, IDC_EDIT12, m_IO[7]);
	DDX_Text(pDX, IDC_EDIT13, m_IO[8]);
	DDX_Text(pDX, IDC_EDIT14, m_IO[9]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJoyStick, CDialog)
	//{{AFX_MSG_MAP(CJoyStick)
	ON_BN_CLICKED(IDC_ConnectJoy, OnConnectJoy)
	ON_BN_CLICKED(IDC_RevData, OnRevData)
	ON_BN_CLICKED(IDC_RevData1, OnRevData1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoyStick message handlers

void CJoyStick::OnConnectJoy() 
{
	HWND Handle=this->m_hWnd;//获取dlg句柄
	if(FAILED(InitDInput1(Handle)))
	{
	}
}

void CJoyStick::OnRevData() 
{
    if(FAILED(UpdateInputState()))
	{
	}
}

void CJoyStick::OnRevData1() 
{
	 SetTimer(1,100,NULL);
}

void CJoyStick::OnTimer(UINT nIDEvent) 
{
    if(FAILED(UpdateInputState()))
	{
	}	
	UpdataJoyStick();
	CDialog::OnTimer(nIDEvent);	
}



HRESULT CJoyStick::InitDInput1(HWND hDlg)
{
   HRESULT hr;
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;
    if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumJoysticksCallback,
                                         NULL, DIEDFL_ATTACHEDONLY ) ) )
        return hr;
    if( NULL == g_pJoystick )
    {
        MessageBox( "没有找到手柄，请确定已安装手柄!");
        return S_OK;
    }
    if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;
    if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE | 
                                                             DISCL_FOREGROUND ) ) )
        return hr; 
    if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback, 
                                                (VOID*)hDlg, DIDFT_ALL ) ) )
        return hr;
    return S_OK;
}
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    HRESULT hr;
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;
    return DIENUM_STOP;
}
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = (HWND)pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;         
    }
    return DIENUM_CONTINUE;
}
HRESULT CJoyStick::UpdateInputState()
{
    HRESULT     hr;



    if( NULL == g_pJoystick ) 
        return S_OK;

    hr = g_pJoystick->Poll(); 
    if( FAILED(hr) )  
    {
        hr = g_pJoystick->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pJoystick->Acquire();
        return S_OK; 
    }

    if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
        return hr; // The device should have been acquired during the Poll()

	/*
    wsprintf( strText, TEXT("%ld"), js.lY ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Y_AXIS ), strText );
    wsprintf( strText, TEXT("%ld"), js.lZ ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Z_AXIS ), strText );
    wsprintf( strText, TEXT("%ld"), js.lRx ); 
    SetWindowText( GetDlgItem( hDlg, IDC_X_ROT ), strText );
    wsprintf( strText, TEXT("%ld"), js.lRy ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Y_ROT ), strText );
    wsprintf( strText, TEXT("%ld"), js.lRz ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Z_ROT ), strText );

    // Slider controls
    wsprintf( strText, TEXT("%ld"), js.rglSlider[0] ); 
    SetWindowText( GetDlgItem( hDlg, IDC_SLIDER0 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rglSlider[1] ); 
    SetWindowText( GetDlgItem( hDlg, IDC_SLIDER1 ), strText );

    // Points of view
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[0] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV0 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[1] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV1 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[2] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV2 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[3] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV3 ), strText );
 
   
    // Fill up text with which buttons are pressed
    str = strText;
    for( int i = 0; i < 128; i++ )
    {
        if ( js.rgbButtons[i] & 0x80 )
            str += wsprintf( str, TEXT("%02d "), i );
    }
    *str = 0;   // Terminate the string 

    SetWindowText( GetDlgItem( hDlg, IDC_BUTTONS ), strText );
*/
    return S_OK;
}



void CJoyStick::UpdataJoyStick()
{
  //  TCHAR       strText[128]; // Device state text
//2个手柄
	int tempdata;
	tempdata=js.lX;
       m_X1=(tempdata/100);
		tempdata=js.lY;
		m_Y1=-(tempdata/100);
			tempdata=js.lZ;
			m_X2=(tempdata/100);
				tempdata=js.lRz;
				m_Y2=-(tempdata/100);

	UpdateData(false);	
    for( int i = 0; i < 10; i++ )
    {
		if (js.rgbButtons[i]==0)
		{
			m_IO[i]=0;
		}
		else
		{
            m_IO[i]=1;
		}
	//	m_IO[i]= js.rgbButtons[i] ;
       // if ( js.rgbButtons[i] & 0x80 )
        //    str += wsprintf( str, TEXT("%02d "), i );
    }
 //   wsprintf( strText, TEXT("%ld"), js.rgdwPOV[0]); 

//    SetWindowText(GetDlgItem( IDC_X_AXIS ), strText );
//SetDlgItemText(IDC_X_AXIS,strText);
}

void CJoyStick::UpdataJoyStick1()
{
//    TCHAR       strText[128]; // Device state text
//2个手柄
	int tempdata;
	tempdata=js.lX;
       m_X1=(tempdata/100);
		tempdata=js.lY;
		m_Y1=-(tempdata/100);
			tempdata=js.lZ;
			m_X2=(tempdata/100);
				tempdata=js.lRz;
				m_Y2=-(tempdata/100);

//	UpdateData(false);	
    for( int i = 0; i < 10; i++ )
    {
				if (js.rgbButtons[i]==0)
		{
			m_IO[i]=0;
		}
		else
		{
            m_IO[i]=1;
		}
		//m_IO[i]= js.rgbButtons[i] ;
       // if ( js.rgbButtons[i] & 0x80 )
        //    str += wsprintf( str, TEXT("%02d "), i );
    }
 //   wsprintf( strText, TEXT("%ld"), js.rgdwPOV[0]); 

//    SetWindowText(GetDlgItem( IDC_X_AXIS ), strText );
//SetDlgItemText(IDC_X_AXIS,strText);
}
