sudo chmod +x pkg/Library/Audio/Plug-Ins/VST3/Futurist.vst3/Contents/MacOS/Futurist
# sudo chown -R root:wheel MacOS

pkgbuild --root ~/Projects/FireSound/Futurist/deploy/MacOS/pkg/ \
         --install-location "/" \
         --identifier "com.FireSound.yourtool" \
         --version "1.0.0" \
         FuturistComponent.pkg

#generate gui
productbuild --synthesize --package FuturistComponent.pkg Distribution.xml
#build installer
productbuild --distribution Distribution.xml --package-path . "FuturistInstaller.pkg"
rm FuturistComponent.pkg









