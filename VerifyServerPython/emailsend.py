import smtplib
from email.mime.text import MIMEText
from email.header import Header
from email.utils import formataddr

'''
def send_email(email, uniqueId):
    print("11113")

    # 邮件内容
    text = "验证码：" + uniqueId + ",请在三分钟内完成注册。"
    message = MIMEText(text, 'plain', 'utf-8')
    message['From'] = Header('songxiao_0928 <songxiao_0928@163.com>', 'utf-8')
    #message['To'] = Header(email, 'utf-8')
    message['To'] = email
    message['Subject'] = Header('来自songxiao_0928的邮件', 'utf-8')

    print("11112")

    # 发送邮件（以QQ邮箱为例）
    try:
        smtp_obj = smtplib.SMTP_SSL('smtp.163.com', 465)
        smtp_obj.login('songxiao_0928@163.com', 'SMnLFwKqyem9kw6W')
        result = smtp_obj.sendmail('songxiao_0928@163.com', email, message.as_string())
        print("邮件发送成功")
    except smtplib.SMTPException as e:
        print(f"邮件发送失败: {e}")
'''


def send_email(recipient_email, verification_code):
    # 邮件配置
    smtp_server = 'smtp.163.com'  # 163邮箱SMTP服务器
    smtp_port = 465  # 非SSL端口，SSL通常用465
    sender_email = 'songxiao_0928@163.com'  # 发件人邮箱
    sender_password = 'SMnLFwKqyem9kw6W'  # 邮箱授权码，不是登录密码

    # 邮件内容
    subject = '来自songxiao_0928的验证码邮件'
    text = f"验证码：{verification_code}，请在三分钟内完成注册。"

    # 创建邮件消息对象
    message = MIMEText(text, 'plain', 'utf-8')
    # 设置发件人信息，formataddr用于处理名称和邮箱的格式
    message['From'] = formataddr(('songxiao_0928', sender_email))
    # 设置收件人信息
    message['To'] = recipient_email
    # 设置邮件主题
    message['Subject'] = Header(subject, 'utf-8')

    try:
        # 连接SMTP服务器并发送邮件
        with smtplib.SMTP_SSL(smtp_server, smtp_port) as server:
            # 登录邮箱
            server.login(sender_email, sender_password)
            # 发送邮件：发件人、收件人、邮件内容
            server.sendmail(sender_email, recipient_email, message.as_string())
        print("邮件发送成功")
        return True
    except Exception as e:
        print(f"邮件发送失败：{str(e)}")
        return False