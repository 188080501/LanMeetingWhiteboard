#include "JasonQt_iOS.h"

// iOS lib import
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

using namespace JasonQt_iOS;

void JasonQt_iOS::pushNotifiction(const QString &message, const QDateTime &dateTime)
{
    UIUserNotificationType types = UIUserNotificationTypeAlert | UIUserNotificationTypeBadge;

    UIUserNotificationSettings *setting = [UIUserNotificationSettings settingsForTypes:types categories:NULL];

    [[UIApplication sharedApplication] registerUserNotificationSettings:setting];

    NSDateComponents *dateComp = [[NSDateComponents alloc] init];
    dateComp.year = dateTime.date().year();
    dateComp.month = dateTime.date().month();
    dateComp.day = dateTime.date().day();
    dateComp.hour = dateTime.time().hour();
    dateComp.minute = dateTime.time().minute();
    dateComp.second = dateTime.time().second();
    dateComp.timeZone = [NSTimeZone systemTimeZone];

    NSCalendar *calendar = [NSCalendar calendarWithIdentifier:NSGregorianCalendar];

    NSDate *date = [calendar dateFromComponents:dateComp];

    UILocalNotification *notifiction = [[UILocalNotification alloc]init];
    notifiction.alertBody = [NSString stringWithFormat:@"%s" , message.toStdString().c_str()];
    notifiction.fireDate = date;

    [[UIApplication sharedApplication]scheduleLocalNotification:notifiction];
}

void JasonQt_iOS::cancelAllNotifictions(void)
{
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber: 0];
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

@interface APLViewController: UIViewController <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{ }
@end

@implementation APLViewController

-(int)showImagePickerForPhotoPicker
{
    [self showImagePickerForSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
    return 5;
}

-(void)showImagePickerForSourceType:(UIImagePickerControllerSourceType)sourceType
{
    UIImagePickerController *imagePickerController = [[UIImagePickerController alloc] init];
    imagePickerController.modalPresentationStyle = UIModalPresentationCurrentContext;
    imagePickerController.sourceType = sourceType;
    imagePickerController.delegate = (id)self;

    UIViewController *rootCtrl = [UIApplication sharedApplication].keyWindow.rootViewController;

    [rootCtrl presentViewController:imagePickerController animated:YES completion:nil];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info valueForKey:UIImagePickerControllerOriginalImage];
    NSData *data = UIImagePNGRepresentation(image);

    const auto &&image2 = QImage::fromData((const unsigned char *)[data bytes], [data length]);

    [picker dismissViewControllerAnimated:YES completion:NULL];

    emit ImagePicker::g_currentImagePicker->imageSelected(image2);
    ImagePicker::g_currentImagePicker = NULL;
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:NULL];
}

- (void)saveImageToPhotos:(UIImage*)savedImage
{
    UIImageWriteToSavedPhotosAlbum(savedImage, self, @selector(image:didFinishSavingWithError:contextInfo:), NULL);
}

- (void)image:(UIImage *)image didFinishSavingWithError: (NSError *) error contextInfo: (void *) contextInfo
{
    emit ImagePicker::g_currentImagePicker->imageSaved(error == NULL);
    ImagePicker::g_currentImagePicker = NULL;

    return;
    if(image); // Just remove the unused warning
    if(contextInfo); // Just remove the unused warning
}

@end

ImagePicker *ImagePicker::g_currentImagePicker = NULL;
void *ImagePicker::g_context = NULL;

void ImagePicker::show(void)
{
    ImagePicker::g_currentImagePicker = this;
    if(!ImagePicker::g_context)
    {
        ImagePicker::g_context = [[APLViewController alloc] init];
    }

    [(id)ImagePicker::g_context showImagePickerForPhotoPicker];
}

void ImagePicker::save(const QImage &targetImage)
{
    ImagePicker::g_currentImagePicker = this;
    if(!ImagePicker::g_context)
    {
        ImagePicker::g_context = [[APLViewController alloc] init];
    }

    QByteArray buf;
    QBuffer buffer(&buf);
    buffer.open(QIODevice::WriteOnly);
    targetImage.save(&buffer, "PNG");

    NSData *data = [[NSData alloc] initWithBytes:(const void *)buf.data() length:buf.size()];
    UIImage *savedImage = [[UIImage alloc] initWithData:data];

    [(id)ImagePicker::g_context saveImageToPhotos:savedImage];

//    [data dealloc];
}
