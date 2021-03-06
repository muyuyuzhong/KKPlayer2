#include "../stdafx.h"
#ifndef AVMiniBottom_H
#define AVMiniBottom_H
namespace SOUI
{
	class CMainDlg;
	class CAVMiniBottom: public SHostWnd
	{
	   public:
		       CAVMiniBottom(CMainDlg *m_pDlgMain);
		       ~CAVMiniBottom();
			   
			   void SetPlayState(bool s);
			   void SetVolume(int volume);
               void OnAVPlay();
			   bool OnSliderVideo(EventArgs *pEvtBase);
			   bool OnSliderAudio(EventArgs *pEvtBase);
			   void OnMuteAudio();
     
				EVENT_MAP_BEGIN()
					EVENT_NAME_COMMAND(L"btn_audio",OnMuteAudio)
					EVENT_NAME_COMMAND(L"AVPlayBtn",OnAVPlay)
					    EVENT_NAME_HANDLER(L"AvAudio",EventSliderPos::EventID,OnSliderAudio)
				        EVENT_NAME_HANDLER(L"slider_video_Seek",EventSliderPos::EventID,OnSliderVideo)
				EVENT_MAP_END()	
				BEGIN_MSG_MAP_EX(CAVMiniBottom)
					CHAIN_MSG_MAP(SHostWnd)
					REFLECT_NOTIFICATIONS_EX()
				END_MSG_MAP()/**/
			    SOUI::CMainDlg *m_pDlgMain;

	};
}
#endif