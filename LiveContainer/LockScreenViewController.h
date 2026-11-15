// === BẮT ĐẦU CODE MÀN HÌNH KHÓA FLEKDECK ===
#import <UIKit/UIKit.h>

@interface FlekLockWindow : UIWindow
@property (nonatomic, strong) NSMutableString *enteredCode;
@property (nonatomic, strong) UILabel *dotsLabel;
@property (nonatomic, strong) UILabel *timeLabel;
@end

@implementation FlekLockWindow

- (instancetype)initWithWindowScene:(UIWindowScene *)windowScene {
    self = [super initWithWindowScene:windowScene];
    if (self) {
        self.windowLevel = UIWindowLevelAlert + 100;
        self.enteredCode = [NSMutableString string];
        
        UIViewController *vc = [[UIViewController alloc] init];
        vc.view.backgroundColor = [UIColor clearColor];
        
        // Lớp kính mờ lấy trọn hình nền màn hình chính phía sau
        UIBlurEffect *blur = [UIBlurEffect effectWithStyle:UIBlurEffectStyleSystemUltraThinMaterialDark];
        UIVisualEffectView *blurView = [[UIVisualEffectView alloc] initWithEffect:blur];
        blurView.frame = vc.view.bounds;
        blurView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [vc.view addSubview:blurView];
        
        // Hiển thị đồng hồ lớn kiểu iOS
        self.timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 80, vc.view.bounds.size.width, 80)];
        self.timeLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        self.timeLabel.textAlignment = NSTextAlignmentCenter;
        self.timeLabel.textColor = [UIColor whiteColor];
        self.timeLabel.font = [UIFont systemFontOfSize:64 weight:UIFontWeightBold];
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:@"HH:mm"];
        self.timeLabel.text = [formatter stringFromDate:[NSDate date]];
        [vc.view addSubview:self.timeLabel];
        
        // Dòng tiêu đề & các chấm mật mã
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 180, vc.view.bounds.size.width, 30)];
        titleLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        titleLabel.text = @"Nhập mật mã FlekDeck";
        titleLabel.textAlignment = NSTextAlignmentCenter;
        titleLabel.textColor = [UIColor colorWithWhite:1.0 alpha:0.9];
        titleLabel.font = [UIFont systemFontOfSize:17 weight:UIFontWeightMedium];
        [vc.view addSubview:titleLabel];
        
        self.dotsLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 215, vc.view.bounds.size.width, 40)];
        self.dotsLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        self.dotsLabel.text = @"○  ○  ○  ○";
        self.dotsLabel.textAlignment = NSTextAlignmentCenter;
        self.dotsLabel.textColor = [UIColor whiteColor];
        self.dotsLabel.font = [UIFont systemFontOfSize:24];
        [vc.view addSubview:self.dotsLabel];
        
        // Bàn phím số 0-9
        UIStackView *mainStack = [[UIStackView alloc] init];
        mainStack.axis = UILayoutConstraintAxisVertical;
        mainStack.spacing = 16;
        mainStack.translatesAutoresizingMaskIntoConstraints = NO;
        [vc.view addSubview:mainStack];
        
        [NSLayoutConstraint activateConstraints:@[
            [mainStack.centerXAnchor constraintEqualToAnchor:vc.view.centerXAnchor],
            [mainStack.topAnchor constraintEqualToAnchor:vc.view.topAnchor constant:280]
        ]];
        
        NSArray *rows = @[@[@"1", @"2", @"3"], @[@"4", @"5", @"6"], @[@"7", @"8", @"9"], @[@"", @"0", @"⌫"]];
        for (NSArray *row in rows) {
            UIStackView *rowStack = [[UIStackView alloc] init];
            rowStack.axis = UILayoutConstraintAxisHorizontal;
            rowStack.spacing = 24;
            for (NSString *key in row) {
                UIButton *btn = [UIButton buttonWithType:UIButtonTypeSystem];
                [btn.widthAnchor constraintEqualToConstant:72].active = YES;
                [btn.heightAnchor constraintEqualToConstant:72].active = YES;
                if (key.length > 0) {
                    [btn setTitle:key forState:UIControlStateNormal];
                    btn.titleLabel.font = [UIFont systemFontOfSize:28 weight:UIFontWeightRegular];
                    [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
                    btn.backgroundColor = [UIColor colorWithWhite:1.0 alpha:0.15];
                    btn.layer.cornerRadius = 36;
                    [btn addTarget:self action:@selector(keyTapped:) forControlEvents:UIControlEventTouchUpInside];
                } else {
                    btn.userInteractionEnabled = NO;
                }
                [rowStack addArrangedSubview:btn];
            }
            [mainStack addArrangedSubview:rowStack];
        }
        self.rootViewController = vc;
    }
    return self;
}

- (void)keyTapped:(UIButton *)sender {
    NSString *title = [sender titleForState:UIControlStateNormal];
    if ([title isEqualToString:@"⌫"]) {
        if (self.enteredCode.length > 0) {
            [self.enteredCode deleteCharactersInRange:NSMakeRange(self.enteredCode.length - 1, 1)];
        }
    } else if (self.enteredCode.length < 4) {
        [self.enteredCode appendString:title];
    }
    
    // Cập nhật dấu chấm hiển thị
    NSMutableArray *dots = [NSMutableArray array];
    for (int i = 0; i < 4; i++) {
        [dots addObject:(i < self.enteredCode.length) ? @"●" : @"○"];
    }
    self.dotsLabel.text = [dots componentsJoinedByString:@"  "];
    
    // Kiểm tra mật mã (Đổi số 1234 tại đây nếu muốn)
    NSString *passcode = @"1234";
    if (self.enteredCode.length == 4) {
        if ([self.enteredCode isEqualToString:passcode]) {
            [UIView animateWithDuration:0.25 animations:^{
                self.alpha = 0;
            } completion:^(BOOL finished) {
                self.hidden = YES;
                [self.enteredCode setString:@""];
                self.dotsLabel.text = @"○  ○  ○  ○";
            }];
        } else {
            // Rung nhẹ khi sai mật mã
            CAKeyframeAnimation *anim = [CAKeyframeAnimation animationWithKeyPath:@"transform.translation.x"];
            anim.values = @[@(-12), @(12), @(-8), @(8), @(0)];
            anim.duration = 0.3;
            [self.dotsLabel.layer addAnimation:anim forKey:@"shake"];
            [self.enteredCode setString:@""];
            self.dotsLabel.text = @"○  ○  ○  ○";
        }
    }
}
@end
static FlekLockWindow *globalLockWindow = nil;
// === KẾT THÚC CODE MÀN HÌNH KHÓA ===
