import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
ApplicationWindow {
 id: win; visible:true; width:1180;height:780; minimumWidth:900;minimumHeight:620; title:"Kindlewall"; color:"#111318"
 property color panel:"#1a1d24"; property color muted:"#9096a3"; property color accent:"#d6ed67"
 header: ToolBar { background: Rectangle{color:"#171a20"} RowLayout{anchors.fill:parent;anchors.leftMargin:24;anchors.rightMargin:24
 Label{text:"kindlewall";font.pixelSize:20;font.bold:true;color:"white"} Item{Layout.fillWidth:true}
 Label{text:backend.files.length? (Math.min(backend.index+1,backend.files.length)+" / "+backend.files.length):"Your wallpaper workspace";color:muted}
 } }
 RowLayout{anchors.fill:parent;anchors.margins:22;spacing:18
 ColumnLayout{Layout.preferredWidth:270;Layout.fillHeight:true;spacing:14
 Rectangle{Layout.fillWidth:true;Layout.preferredHeight:125;radius:12;color:panel; Column{anchors.fill:parent;anchors.margins:16;spacing:8;Label{text:"WORKFLOW";color:muted;font.pixelSize:12;font.bold:true}Label{text:backend.files.length?"Review your collection":"Start with a wallpaper folder";color:"white";font.pixelSize:17;font.bold:true;wrapMode:Text.WordWrap}Label{text:backend.files.length?"One image at a time · originals stay safe":"Choose a folder to begin cropping";color:muted;wrapMode:Text.WordWrap}}}
 Button{text:"＋   Choose image folder";Layout.fillWidth:true;onClicked:sourceDialog.open()}
 Rectangle{Layout.fillWidth:true;height:1;color:"#2a2e37"}
 Label{text:"OUTPUT SIZE";color:muted;font.bold:true;font.pixelSize:12}
 RowLayout{TextField{id:wid;text:backend.targetWidth;Layout.fillWidth:true;placeholderText:"Width";validator:IntValidator{bottom:1;top:10000} onEditingFinished:backend.setResolution(parseInt(text),parseInt(hei.text))}Label{text:"×";color:muted}TextField{id:hei;text:backend.targetHeight;Layout.fillWidth:true;placeholderText:"Height";validator:IntValidator{bottom:1;top:10000} onEditingFinished:backend.setResolution(parseInt(wid.text),parseInt(text))}}
 Button{text:"Choose output folder";Layout.fillWidth:true;onClicked:outDialog.open()}
 Label{text:backend.outputDir;color:muted;elide:Text.ElideMiddle;Layout.fillWidth:true}
 Label{text:"KINDLE TRANSFER";color:muted;font.bold:true;font.pixelSize:12}
 Button{text:"Set Kindle folder";Layout.fillWidth:true;onClicked:kindleDialog.open()}
 Label{text:backend.kindleDir||"Folder not set";color:muted;elide:Text.ElideMiddle;Layout.fillWidth:true}
 Button{text:"↑   Push finished wallpapers";Layout.fillWidth:true;enabled:!!backend.kindleDir;highlighted:true;onClicked:backend.pushToKindle()}
 Item{Layout.fillHeight:true}
 Label{text:"Zoom  " + backend.zoom + "%";color:muted}
 Slider{Layout.fillWidth:true;from:50;to:200;stepSize:5;value:backend.zoom;onMoved:backend.setZoom(Math.round(value));WheelHandler{onWheel:event=>{backend.setZoom(backend.zoom+(event.angleDelta.y>0?5:-5));event.accepted=true}}}
 Label{text:"Tilt  " + backend.angle + "°";color:muted}
 Slider{Layout.fillWidth:true;from:-180;to:180;stepSize:1;value:backend.angle;onMoved:backend.setAngle(Math.round(value));WheelHandler{onWheel:event=>{backend.setAngle(backend.angle+(event.angleDelta.y>0?1:-1));event.accepted=true}}}
 Label{text:backend.message;color:muted;wrapMode:Text.WordWrap;Layout.fillWidth:true}
 }
 ColumnLayout{Layout.fillWidth:true;Layout.fillHeight:true;spacing:14
 Rectangle{Layout.fillWidth:true;Layout.fillHeight:true;radius:14;color:panel
 Item{anchors.fill:parent;anchors.margins:24
 Image{id:photo;anchors.fill:parent;source:backend.currentPath ? "image://preview/"+backend.index+"-"+backend.zoom+"-"+backend.angle : "";fillMode:Image.PreserveAspectFit;asynchronous:true;cache:false}
 Rectangle{visible:backend.currentPath!=="";width:Math.min(photo.paintedWidth,photo.paintedHeight*backend.targetWidth/backend.targetHeight);height:width*backend.targetHeight/backend.targetWidth;x:(parent.width-photo.paintedWidth)/2+backend.cropLeft/10000*(photo.paintedWidth-width);y:(parent.height-photo.paintedHeight)/2+backend.cropTop/10000*(photo.paintedHeight-height);color:"transparent";border.width:2;border.color:accent;Rectangle{anchors.fill:parent;color:"#050609";opacity:.55;z:-1}Text{anchors.centerIn:parent;text:"CROP PREVIEW";color:"white";font.bold:true;font.pixelSize:12}}
 Column{anchors.centerIn:parent;visible:!backend.currentPath;spacing:12;Label{anchors.horizontalCenter:parent.horizontalCenter;text:"▧";font.pixelSize:44;color:accent}Label{anchors.horizontalCenter:parent.horizontalCenter;text:"Your next wallpaper starts here";color:"white";font.pixelSize:20;font.bold:true}Label{anchors.horizontalCenter:parent.horizontalCenter;text:"Pick a folder and frame each image with the arrow keys";color:muted}}
 }
 }
 RowLayout{Layout.fillWidth:true;Label{text:"← / ↑ / → / ↓  Adjust crop     Enter  Save & next     Backspace  Skip     Ctrl+Z  Undo";color:muted;font.pixelSize:12;Layout.fillWidth:true}Button{text:"Skip";onClicked:backend.skip()}Button{text:"Save crop  ↵";highlighted:true;enabled:!!backend.currentPath;onClicked:backend.finish()}}
 }
 }
 FolderDialog{id:sourceDialog;title:"Choose wallpaper folder";onAccepted:backend.loadFolder(selectedFolder)}
 FolderDialog{id:kindleDialog;title:"Choose Kindle wallpapers folder";onAccepted:backend.chooseKindle(selectedFolder)}
 FolderDialog{id:outDialog;title:"Choose output folder";onAccepted:backend.chooseOutput(selectedFolder)}
 Shortcut{sequence:"Return";onActivated:backend.finish()}Shortcut{sequence:"Backspace";onActivated:backend.skip()}Shortcut{sequence:"Ctrl+Z";onActivated:backend.undo()}
 Shortcut{sequence:"Left";onActivated:backend.moveCrop(-1,0)}Shortcut{sequence:"Right";onActivated:backend.moveCrop(1,0)}Shortcut{sequence:"Up";onActivated:backend.moveCrop(0,-1)}Shortcut{sequence:"Down";onActivated:backend.moveCrop(0,1)}
 Shortcut{sequence:"Ctrl+O";onActivated:sourceDialog.open()}
}
