#import <Foundation/Foundation.h>
#import <FxPlug/FxPlugSDK.h>
#import <stdio.h>

int main() {
  printf("PS: %s\n", [kFxPropertyKey_PixelTransformSupport UTF8String]);
  printf("DOSC: %s\n", [kFxPropertyKey_DesiredOSCIDs UTF8String]);
  return 0;
}
