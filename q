commit 6517028587e21565c274e85452c1ce8c21752392
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Wed Apr 29 12:26:13 2015 +0800

    Set starting empathy thread as detachable in order to get rid of
    zombie thread.

commit 262f7a2f7c79719c0f223f61e8156fb19676424b
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Wed Apr 29 04:05:45 2015 +0800

    update release

commit ec57ff25b8b603b0493f049a23e3ad930733621d
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Wed Apr 29 10:51:51 2015 +0800

    1. Modify the auto-refresh time interval to 10 seconds.
    2. Add a new thread to start up "empathy" once the user ID
        verification is okay.

commit 014fd490b801d57172143954e0c814bbaf581a9f
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Wed Apr 29 09:34:31 2015 +0800

    Changed assistor.spec file. It not only install the binary to
    /usr/local/bin, but also assistor.desktop to /usr/share/applications
    and assistor.png to /usr/share/assistor/pixmaps.

commit b3887c082d4ed9413b38d3f158069dc8ff814bc7
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Tue Apr 28 19:19:10 2015 +0800

    There are four changes with regard to this commit:
        1. Display the remote technical supporter's contacting info.
        2. Add auto-refresh function after first time clicking the
            "refresh" button in the dialog window.
        3. User can start up this app through desktop Startup Menu.
        4. User's IP address can be read from local configuration
            files, that is "/etc/sysconfig/network-script/ifcfg*".

commit c82ecd53d540e733ebe512bb0bc513555c53f57a
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Wed Apr 22 17:18:51 2015 +0800

    modified assistor.spec

commit 0f260e4526155872f0e1f48c8bd2eb53af2c3219
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Wed Apr 22 16:58:22 2015 +0800

    use lookback ip instead of the ture ip address of the NIC, this is
    a bug and need to fix it later.

commit 82e18d808c91edf3920f297efcb6d4b33919c589
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Wed Apr 22 15:20:16 2015 +0800

    Submit for build06

commit 54cb5529ac13b454572d687389bff68c0f612c41
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Wed Apr 22 05:47:59 2015 +0800

    modified assistor.spec

commit 2ae7b998a14f196999e836303742bbc95cf3aabc
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Mon Apr 20 05:26:01 2015 +0800

    assistor.spec

commit 9f7a774d6e58ec211aa3479ddac0c0c99986dd45
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Mon Apr 20 03:13:50 2015 +0800

    modified packages-assistor.sh

commit 5959ba7236acfb13f0b4542c3f3517d8ce8c9cc9
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Fri Apr 17 17:03:47 2015 +0800

    Fixed bug 37672. assistor only allow run once for each user.
    Add a README file which directs how to install this software
    and how to use it.

commit 14c7af3d2730b87d8cebb95b016cd15ccbe74390
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Fri Apr 17 17:05:09 2015 +0800

    add BuildReq to assistor.spec

commit 9373f2db75bfa445145f2e0cd1c8c3e9303c91e5
Author: zhuo.wang <zhuo.wang@cs2c.com.cn>
Date:   Fri Apr 17 15:45:13 2015 +0800

    add packages-assistor.sh

commit 8c9e94d1177176cb583d65b5e060851a30a456b2
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Fri Apr 17 14:37:01 2015 +0800

    Modified assistor.spec file to generate rpm package and srpm package.

commit 347dde98bcf453c5c08306d0e789a070b5c29f98
Author: eastmoutain <east.moutain.yang@gmail.com>
Date:   Fri Apr 17 10:27:28 2015 +0800

    The codes are pushed based on build05, the function is not completed.

commit af997c3fb85b0f5b9fc40239fc156129d55df3f1
Author: jiangsheng.yang <jiangsheng.yang@cs2c.com.cn>
Date:   Wed Apr 15 11:20:59 2015 +0800

    summit spec file
