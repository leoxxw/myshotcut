#ifndef PUBLICINFO_H
#define PUBLICINFO_H

#endif // PUBLICINFO_H
#include <QString>
enum enReturnType
{
    // 0：失败，具体失败原因请调用DoGetLastError
    ERT_FALSE = 0,
    ERT_TRUE,
    ERT_CANCEL,
    // 生成新的TGT成功，外部需要注销该TGT
    ERT_NewTGT = 5,

    // －1接口凭证过期
    ERT_INTERFACE_CERTIFICATE_OVERDUE = -1,
    // －2接口凭证无效
    ERT_INTERFACE_CERTIFICATE_INVALID = -2,

    // －3用户凭证过期
    ERT_USER_CERTIFICATE_OVERDUE = -3,
    // －4用户凭证无效
    ERT_USER_CERTIFICATE_INVALID = -4,

    // －5返回值空间不足，为该返回值时，长度参数作为输入输出时，该参数为所需空间
    ERT_RETURN_VALUE_SIZE_NOT_ENOUGH = -5,

    // －6 URL无效，如webservice地址无效，页面url无效等
    ERT_URL_INVALID = -6,

    // －7无效的业务系统ID或接入码
    ERT_ERROR = -7,

    // 自动登录失败，由外部调用登录界面
    ERT_AVOID_LOGIN_FALSE = -8,
};
enum enLogType
{
    // 仅仅输出错误日志
    ELT_ONLY_ERROR = 0,
    // 输出所有日志
    ELT_ALL,
};

enum enWebReturnCode
{
    EWRC_SUCC = 0,
    // 参数错误
    EWRC_3100 = 3100,
    // 无效的用户，请联系管理员。
    EWRC_3101 = 3101,
    // 无效的密码。
    EWRC_3102 = 3102,
    // 该账户已被停用，请联系管理员启用账户。
    EWRC_3103 = 3103,
    // 用户无权限
    EWRC_3104 = 3104,
    // 无效的票据授权票。
    EWRC_3105 = 3105,
};
//nResourceMajorType： 要打开/保存的资源类型过滤，枚举值如下，只参使用一个值：
//    0x00 自动确定
//    0x01 音频
//    0x02 视频
//    0x04 图片
//    0x08 文档
//    0x10 其它
//    0xFF 所有
enum enResourceMajorType
{
    ERMT_AUTO = 0,  // 自动确定
    ERMT_Audio = 1,
    ERMT_Video = 2,
    ERMT_Picture = 4,
    ERMT_Document = 8,
    ERMT_Other = 16,
    ERMT_ALL = 255,
};

enum enWebFlag
{
    // 对于打开对话框，则有以下标志
    // 0x00000200  允许多选
    EWF_WEB_OPEN_FLAG_DEFAULT = 0x00000000,
    EWF_WEB_OPEN_MUTI_SELECT  = 0x00000200,

    // 对于保存对话框，则有以下标志：
    // 0x00000002  覆盖提示（页面提示是否覆盖提示）
    EWF_WEB_SAVE_FLAG_DEFAULT = 0x00000000,
    EWF_WEB_SAVE_COVER_PROMPT = 0x00000002,
};


//0x0001  强制覆盖（不做校验，强制全部下载并覆盖本地）
//0x0002  本地优先　(不做校验，如果本地没有的则下载，否则不下载)
//0x0004   服务端优先（启用校验，如果相同，则不下载，如果不同或本地缺少，则下载到本地）
//0x0008   提示（启用校验，提示由用户选择使用服务端版本或者本地版本-选项可以记住）
enum enDownloadFalg
{
    EDFalg_FORCE_COVER = 1,
    EDFalg_LOCAL_FIRST = 2,
    EDFalg_SERVER_FIRST = 4,
    EDFalg_PROMPT = 8
};

// 资源类型 工程使用其他
//    -1 目录
//    0 音频
//    1 视频
//    2 图片
//    3 文档
//    4 其它
enum enResourceType
{
    EREST_Directory = -1,
    EREST_Audio,
    EREST_Video,
    EREST_Picture,
    EREST_Document,
    EREST_Other,
};
struct ResourceInfo
{
    QString m_strResourceName;
    QString m_strResourceID;
    QString m_strParentID;
    QString m_strOwnerID;
    QString m_strOwnerType;
};
