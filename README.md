//-------------------------------------------------------------//
//---------------	20161250 이지호           ---------------//
//---------------	컴퓨터공학설계및실습I         ---------------//
//---------------	기말 프로젝트               ---------------//
//---------------	< Amazing BeatFall! >   ---------------//
//-------------------------------------------------------------//

// presentation video는 Youtube에 업로드하였습니다.
// https://youtu.be/em3n4LfL2G4

// 실행 방법
// for Mac:
//	/bin/Amazing BeatFall (for Mac) 실행 (더블 클릭)
// for Windows:
//	/bin/Amazing BeatFall (for Windows).exe 실행 (더블 클릭)

// 소스 코드는 /src 에 있습니다!
// 프로그램에 필요한 모든 데이터 파일은 /data 에 있습니다.
	- .abf 파일       : /data/beatfall
	- font(.ttf) 파일 : /data/fonts
	- 사운드 샘플 파일   : /data/samples
	- 휴지통 아이콘      : /data


// Introduction of < Amazing BeatFall! > :
	-   9-10주차 실습 < WaterFall > 을 응용하여 제작한 프로그램으로, 사용자가 스크린에 생성한 BeatBall이 Pedal에 부딪히면 여러 악기 샘플이 재생되어 하나의 Beat & Melody 를 시각적으로 구현하는 Program 입니다.
	-   사용자는 BeatBall을 낙하시키기 전, Drag & Drop으로 Pedal의 위치와 각도, BeatBall이 생성될 Generator의 위치 등을 조절할 수 있습니다.
	-   Example 시연을 위한 몇 개의 Preset File이 BeatFall/bin/data/beatfall에 저장되어 있습니다. BeatBall, Pedal을 자유롭게 배치하여 나만의 Beat & Melody를 만들어보세요!

// How to run & play < Amazing BeatFall! >
    -   [Mouse Drag & Drop]
      정사각형 모양의 Generator, 직선 모양의 Pedal과 Path를 Drag & Drop으로 이동할 수 있습니다.
      또한, Pedal과 Path의 양 끝 점을 Drag하면 회전시킬 수 있습니다.
      화면 오른쪽의 Side Menubar로부터 Drag & Drop으로 구조물을 추가할 수도 있고, 필드에 있는 구조물을 'Trash'로 끌어다 놓아 제거할 수도 있습니다.
    -   [Space Bar] : Play / Stop
      BeatFall을 시작하거나 일시정지할 수 있습니다.
    -   [Q] : Quit
      < Amazing BeatFall! > program을 종료합니다.
    -   [S] : Save
      현재 BeatFall의 정보를 .abf 확장자 파일로 저장합니다.
    -   [L] : Load
      .abf 파일을 불러와 화면에 띄웁니다.
    -   [C] : Clear
      현재 BeatFall의 모든 구조물을 삭제합니다.
    -   [UP / DOWN] : Volume Up & Down
      볼륨을 조절합니다. (0 - 100)
    -   [< >] : BPM Up & Down
      BPM을 조절합니다. (90 - 180)
      

// Tip.
    -   화면 좌측 하단에 현재 프로그램 상태, 시스템 알림 메시지, 키보드 조작법을 확인할 수 있습니다.
    -   프로그램 실행 마다 각 구조물의 색상, Pedal의 Sound Sample이 랜덤하게 설정됩니다.
