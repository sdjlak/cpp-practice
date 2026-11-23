# -*- coding: utf-8 -*-
"""生成《大二 C++ 实习作战手册》DOCX。"""

from docx import Document
from docx.shared import Pt, Cm, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.enum.section import WD_SECTION
from docx.oxml.ns import qn
from docx.oxml import OxmlElement

OUT = r"D:\c++\大二C++实习作战手册.docx"

INK = "1F3A5F"      # 主色：深海蓝
ACCENT = "C0504D"   # 强调色
HEAD_FILL = "DCE6F1"  # 表头底色
NOTE_FILL = "FFF6E5"  # 提示框底色
GRAY = "595959"

EA_BODY = "宋体"
EA_HEAD = "黑体"
ASCII_FONT = "Calibri"


# ---------------- 基础工具 ----------------
def style_font(style, ascii_name, ea_name, size, bold=None, color=None):
    f = style.font
    f.name = ascii_name
    f.size = Pt(size)
    if bold is not None:
        f.bold = bold
    if color:
        f.color.rgb = RGBColor.from_string(color)
    rPr = style.element.get_or_add_rPr()
    rFonts = rPr.find(qn("w:rFonts"))
    if rFonts is None:
        rFonts = OxmlElement("w:rFonts")
        rPr.append(rFonts)
    rFonts.set(qn("w:ascii"), ascii_name)
    rFonts.set(qn("w:hAnsi"), ascii_name)
    rFonts.set(qn("w:eastAsia"), ea_name)


def set_run(run, size=10.5, bold=False, color=None, ea=EA_BODY, ascii_name=ASCII_FONT, italic=False):
    run.font.size = Pt(size)
    run.font.bold = bold
    run.font.italic = italic
    run.font.name = ascii_name
    if color:
        run.font.color.rgb = RGBColor.from_string(color)
    rPr = run._element.get_or_add_rPr()
    rFonts = rPr.find(qn("w:rFonts"))
    if rFonts is None:
        rFonts = OxmlElement("w:rFonts")
        rPr.append(rFonts)
    rFonts.set(qn("w:ascii"), ascii_name)
    rFonts.set(qn("w:hAnsi"), ascii_name)
    rFonts.set(qn("w:eastAsia"), ea)


def H1(doc, text):
    p = doc.add_paragraph(style="Heading 1")
    p.add_run(text)
    return p


def H2(doc, text):
    p = doc.add_paragraph(style="Heading 2")
    p.add_run(text)
    return p


def P(doc, text, size=10.5, bold=False, color=None, indent=0, space_after=5,
      align=None, line=1.4, first_line=0):
    p = doc.add_paragraph()
    r = p.add_run(text)
    set_run(r, size=size, bold=bold, color=color)
    pf = p.paragraph_format
    pf.space_after = Pt(space_after)
    pf.space_before = Pt(0)
    pf.line_spacing = line
    if indent:
        pf.left_indent = Cm(indent)
    if first_line:
        pf.first_line_indent = Cm(first_line)
    if align is not None:
        p.alignment = align
    return p


def BULLET(doc, text, size=10.5, level=0):
    p = doc.add_paragraph(style="List Bullet")
    r = p.add_run(text)
    set_run(r, size=size)
    pf = p.paragraph_format
    pf.space_after = Pt(3)
    pf.line_spacing = 1.35
    pf.left_indent = Cm(0.75 + 0.5 * level)
    return p


def NUM(doc, text, size=10.5):
    p = doc.add_paragraph(style="List Number")
    r = p.add_run(text)
    set_run(r, size=size)
    pf = p.paragraph_format
    pf.space_after = Pt(3)
    pf.line_spacing = 1.35
    pf.left_indent = Cm(0.75)
    return p


def NUML(doc, num, text, size=10.5):
    """显式编号条目，避免跨章节共用 List Number 计数器。"""
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.left_indent = Cm(1.0)
    pf.first_line_indent = Cm(-0.6)
    pf.space_after = Pt(3)
    pf.line_spacing = 1.35
    r = p.add_run(f"{num}.  ")
    set_run(r, size=size)
    r2 = p.add_run(text)
    set_run(r2, size=size)
    return p


def shade_cell(cell, hexcolor):
    tcPr = cell._tc.get_or_add_tcPr()
    shd = OxmlElement("w:shd")
    shd.set(qn("w:val"), "clear")
    shd.set(qn("w:color"), "auto")
    shd.set(qn("w:fill"), hexcolor)
    tcPr.append(shd)


def vcenter(cell):
    tcPr = cell._tc.get_or_add_tcPr()
    va = OxmlElement("w:vAlign")
    va.set(qn("w:val"), "center")
    tcPr.append(va)


def table(doc, rows, widths, header=True, size=9.5, zebra=True, align_center=None):
    """rows: list of list(str)。单元格内可用 \n 分段。"""
    n_col = len(rows[0])
    t = doc.add_table(rows=len(rows), cols=n_col)
    t.style = "Table Grid"
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    t.autofit = False
    tblPr = t._tbl.tblPr
    layout = OxmlElement("w:tblLayout")
    layout.set(qn("w:type"), "fixed")
    tblPr.append(layout)

    for ri, row in enumerate(rows):
        t.rows[ri].height = Cm(0.6)
        for ci, val in enumerate(row):
            cell = t.cell(ri, ci)
            cell.width = Cm(widths[ci])
            vcenter(cell)
            is_head = header and ri == 0
            if is_head:
                shade_cell(cell, HEAD_FILL)
            elif zebra and ri % 2 == 0:
                shade_cell(cell, "F5F8FC")
            lines = str(val).split("\n")
            cell.paragraphs[0].text = ""
            for li, line in enumerate(lines):
                p = cell.paragraphs[0] if li == 0 else cell.add_paragraph()
                r = p.add_run(line)
                set_run(r, size=size, bold=is_head, color=INK if is_head else None)
                p.paragraph_format.space_after = Pt(1)
                p.paragraph_format.space_before = Pt(1)
                p.paragraph_format.line_spacing = 1.15
                if align_center and ci in align_center:
                    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    # 表头行重复
    if header and len(t.rows) > 1:
        trPr = t.rows[0]._tr.get_or_add_trPr()
        th = OxmlElement("w:tblHeader")
        th.set(qn("w:val"), "true")
        trPr.append(th)
    return t


def callout(doc, title, body, fill=NOTE_FILL):
    t = doc.add_table(rows=1, cols=1)
    t.style = "Table Grid"
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    t.autofit = False
    cell = t.cell(0, 0)
    cell.width = Cm(16.0)
    shade_cell(cell, fill)
    p0 = cell.paragraphs[0]
    p0.text = ""
    r = p0.add_run(title)
    set_run(r, size=10, bold=True, color=ACCENT)
    p0.paragraph_format.space_after = Pt(2)
    p0.paragraph_format.space_before = Pt(2)
    for line in body.split("\n"):
        p = cell.add_paragraph()
        r = p.add_run(line)
        set_run(r, size=10)
        p.paragraph_format.space_after = Pt(2)
        p.paragraph_format.line_spacing = 1.35
    doc.add_paragraph().paragraph_format.space_after = Pt(2)


def spacer(doc, pt=6):
    p = doc.add_paragraph()
    p.paragraph_format.space_after = Pt(0)
    p.paragraph_format.space_before = Pt(0)
    p.paragraph_format.line_spacing = Pt(pt)


def add_page_number(section):
    footer = section.footer
    p = footer.paragraphs[0]
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r = p.add_run()
    set_run(r, size=9, color=GRAY)
    fld1 = OxmlElement("w:fldChar")
    fld1.set(qn("w:fldCharType"), "begin")
    instr = OxmlElement("w:instrText")
    instr.set(qn("xml:space"), "preserve")
    instr.text = " PAGE "
    fld2 = OxmlElement("w:fldChar")
    fld2.set(qn("w:fldCharType"), "end")
    r._r.append(fld1)
    r._r.append(instr)
    r._r.append(fld2)


def hrule(doc, color=INK, size=6):
    p = doc.add_paragraph()
    pPr = p._p.get_or_add_pPr()
    pBdr = OxmlElement("w:pBdr")
    bottom = OxmlElement("w:bottom")
    bottom.set(qn("w:val"), "single")
    bottom.set(qn("w:sz"), str(size))
    bottom.set(qn("w:space"), "1")
    bottom.set(qn("w:color"), color)
    pBdr.append(bottom)
    pPr.append(pBdr)
    p.paragraph_format.space_after = Pt(2)
    p.paragraph_format.space_before = Pt(0)


# ---------------- 文档构建 ----------------
doc = Document()

sec = doc.sections[0]
sec.page_width = Cm(21.0)
sec.page_height = Cm(29.7)
sec.top_margin = Cm(2.2)
sec.bottom_margin = Cm(2.2)
sec.left_margin = Cm(2.4)
sec.right_margin = Cm(2.4)
add_page_number(sec)

style_font(doc.styles["Normal"], ASCII_FONT, EA_BODY, 10.5, color="262626")
style_font(doc.styles["Heading 1"], ASCII_FONT, EA_HEAD, 17, bold=True, color=INK)
style_font(doc.styles["Heading 2"], ASCII_FONT, EA_HEAD, 12.5, bold=True, color=INK)
doc.styles["Heading 1"].paragraph_format.space_before = Pt(20)
doc.styles["Heading 1"].paragraph_format.space_after = Pt(8)
doc.styles["Heading 2"].paragraph_format.space_before = Pt(12)
doc.styles["Heading 2"].paragraph_format.space_after = Pt(4)

doc.core_properties.title = "大二C++实习作战手册"
doc.core_properties.subject = "2026.11 - 2027.7 C++ 实习求职规划"
doc.core_properties.author = "鲸鱼娘"

# ===== 封面 =====
spacer(doc, 40)
p = doc.add_paragraph()
p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run("C++ 实习作战手册")
set_run(r, size=32, bold=True, color=INK, ea=EA_HEAD)
p.paragraph_format.space_after = Pt(6)

p = doc.add_paragraph()
p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run("大二上学期 → 2027 年夏")
set_run(r, size=15, color=ACCENT, ea=EA_HEAD)
p.paragraph_format.space_after = Pt(2)

hrule(doc)
spacer(doc, 10)

info = [
    ["目标", "2027 年 7–8 月，拿到一份 C++ 相关实习（游戏方向优先）"],
    ["身份", "西南科技大学 · 大二上学期 · 蓝桥杯 B 组已报名"],
    ["起点", "熟悉 STL 与基础算法，尚无工程与并发经验"],
    ["执行周期", "2026.11.18 — 2027.07.11（共 33 周）"],
    ["制定日期", "2026 年 11 月 18 日"],
]
t = doc.add_table(rows=len(info), cols=2)
t.style = "Table Grid"
t.autofit = False
t.alignment = WD_TABLE_ALIGNMENT.CENTER
for i, (k, v) in enumerate(info):
    t.rows[i].height = Cm(0.85)
    c0, c1 = t.cell(i, 0), t.cell(i, 1)
    c0.width = Cm(3.2)
    c1.width = Cm(12.4)
    shade_cell(c0, HEAD_FILL)
    vcenter(c0)
    vcenter(c1)
    c0.paragraphs[0].text = ""
    r = c0.paragraphs[0].add_run(k)
    set_run(r, size=10, bold=True, color=INK)
    c0.paragraphs[0].paragraph_format.line_spacing = 1.2
    c1.paragraphs[0].text = ""
    r = c1.paragraphs[0].add_run(v)
    set_run(r, size=10)
    c1.paragraphs[0].paragraph_format.line_spacing = 1.2

spacer(doc, 18)
p = doc.add_paragraph()
p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run("“把焦虑换成验收标准，每周交一次货。”")
set_run(r, size=11, italic=True, color=GRAY)
p.paragraph_format.space_after = Pt(4)

doc.add_page_break()

# ===== 一、目标与现状 =====
H1(doc, "一、目标与现状")
H2(doc, "1.1 目标拆解")
P(doc, "总目标：在 2027 年 7–8 月获得一段 C++ 实习，用它撬动大三秋招的更高起点。")

table(doc, [
    ["层级", "具体指标", "达成时间"],
    ["保底", "成都中小游戏公司或软件公司的 C++ 实习岗", "2027-07"],
    ["达标", "成都中大型游戏公司（完美世界、西山居、tap4fun 等）实习", "2027-07"],
    ["冲刺", "大厂成都分部日常实习（腾讯天美、网易、华为成研所）", "2027-07"],
    ["简历资产", "1 个工程级 C++ 项目 + 1 个游戏 Demo + 蓝桥杯省一", "2027-04"],
], widths=[2.6, 10.4, 3.0], align_center=[0, 2])

spacer(doc)

H2(doc, "1.2 现状盘点")
P(doc, "把差距写清楚，比喊口号有用。下面每一行都是要补的清单，不是自我否定。")
table(doc, [
    ["能力项", "当前状态", "需补齐到"],
    ["C++ 语言", "会用 STL 做题，没写过类设计", "RAII、智能指针、move 语义、STL 底层"],
    ["工程能力", "无", "CMake、Git、gdb、Linux 命令行"],
    ["并发编程", "无", "thread、mutex、条件变量、线程池"],
    ["网络编程", "无", "socket、epoll、Reactor 模型"],
    ["算法", "有基础手感（前缀和、滑动窗口）", "蓝桥杯省一 + 面试手撕不卡"],
    ["项目经历", "无", "2 个能讲 20 分钟的项目"],
    ["实习经历", "无", "1 段完整的实习"],
], widths=[2.6, 7.0, 6.4])

spacer(doc)

H2(doc, "1.3 关键时间锚点")
table(doc, [
    ["时间", "事件", "对计划的影响"],
    ["2026.11.18", "计划启动", "环境搭建与 C++ 工程化起步"],
    ["2026.11.24", "CCF CSP 团体报名截止", "错过要多花 200 元，务必先找学习委员"],
    ["2026.12.13", "CCF CSP 认证（第 44 次）", "算法试水战，目标 200 分拿证"],
    ["2026.12 底", "寒假开始", "项目与蓝桥杯专题并行的关键期"],
    ["2027.01", "大二上期末", "不能挂科，预留两周复习"],
    ["2027.03 中旬", "简历 v1 完成，开始投递", "投递窗口正式打开"],
    ["2027.04 中旬", "蓝桥杯省赛", "占用 1 天，考完立刻回主线"],
    ["2027.06", "期末考试 + 面试高峰", "全年最紧张的一个月"],
    ["2027.07–08", "实习入职", "目标达成"],
    ["2028.03–05", "大三下 · 暑期实习主战场", "已有实习经历，起点完全不同"],
], widths=[2.8, 6.0, 7.2], align_center=[0])

# ===== 二、现实约束 =====
H1(doc, "二、四条现实约束")
P(doc, "这几条决定了策略，先认清再干活，能少走很多弯路。")

H2(doc, "2.1 你投的是「日常实习」，不是「暑期实习项目」")
P(doc, "大厂挂「2027 暑期实习」名头的校招项目，普遍面向大三与研二。大二学生能够进入的，几乎都是"
       "日常实习——全年开放、随时可投、时长灵活。这不是坏消息：你不必等窗口期，三月就能开投。")

H2(doc, "2.2 实习时长是最大门槛，不是能力")
P(doc, "企业普遍要求连续实习三个月以上，而暑假只有两个月。这是大二学生被拒的第一原因，与技术水平关系不大。")
callout(doc, "标准话术（面试与投递信里都这样讲）",
        "「暑假可以全职实习两个月（七月初到八月底），开学后能每周到岗三天或远程继续，"
        "可以稳定做到年底。」\n"
        "关键在于给出「连续性」，而不是强调「只能做两个月」。")

H2(doc, "2.3 双非加重学历劣势，只能用作品对冲")
P(doc, "游戏与嵌入式行业对学校的宽容度高于互联网后端。你手里能打的牌只有三张："
       "能跑能玩的项目、压测出来的数据、可查的提交记录。学历改不了，证据可以自己造。")

H2(doc, "2.4 认清实习的真实价值")
P(doc, "大二实习大概率是辅助性工作，不会让你写核心模块。真实收益是：简历上一段经历、"
       "一次真实工程环境的体验、面试时有话可聊。目标是「进入行业并完整走完一段实习」，"
       "而不是「做出成绩」。心态摆错，干两周就会委屈。")

# ===== 三、总路线图 =====
H1(doc, "三、总路线图（33 周）")
table(doc, [
    ["阶段", "周次", "时间", "主线", "副线"],
    ["一 · 打底", "W1–W6", "2026.11.18\n–12.28", "C++ 工程化\n+ CSP 真题训练", "环境搭建\nGit 建仓"],
    ["二 · 并发", "W7–W12", "2026.12.29\n–2027.02.07", "多线程 + 线程池", "蓝桥杯专题冲刺"],
    ["三 · 网络", "W13–W18", "2027.02.08\n–03.21", "Linux + epoll + HTTP", "简历 v1 · 开投"],
    ["四 · 实战", "W19–W24", "2027.03.22\n–05.02", "投递 + 面试 + 补漏", "UE5 小 Demo"],
    ["五 · 冲刺", "W25–W33", "2027.05.03\n–07.11", "面试冲刺 + 入职", "项目打磨"],
], widths=[2.6, 1.9, 3.4, 4.3, 3.8], align_center=[0, 1, 2])

spacer(doc)
callout(doc, "排期的三条硬规则",
        "1. 顺序不可颠倒：先语言，再并发，再网络。跳过语言直接学框架，面试一问就穿。\n"
        "2. W19 之后双线并行：主线投游戏服务端与 C++ 客户端，副线投纯客户端 Demo 岗。\n"
        "3. 每个阶段结束必须交付「可验证的东西」，没有达标的不要进入下一阶段。")

# ===== 四、阶段计划 =====
H1(doc, "四、分阶段执行计划")

H2(doc, "阶段一 · 语言与工程地基（W1–W6，2026.11.18 – 12.28）")
P(doc, "本周就要开始。这一阶段的目标不是「学完 C++」，而是「从刷题的人变成写工程的人」。", bold=True)
P(doc, "任务清单：", bold=True)
BULLET(doc, "环境：VS Code + MinGW-w64(g++) + CMake + Git，跑通第一个工程并完成首次提交。")
BULLET(doc, "C++ 核心：类与对象、构造与析构、拷贝构造与赋值运算符、RAII 思想、智能指针、move 语义。")
BULLET(doc, "STL 底层：vector 扩容机制、map 与 unordered_map 的底层结构、迭代器失效场景。")
BULLET(doc, "算法：十二月十三日 CSP 真题训练（以第 1、2 题为主），考完再回到 Hot 100 基础题型。")
BULLET(doc, "课程：保证不挂科，绩点保持可用状态。")
P(doc, "验收标准（做不到就回头补，别往下走）：", bold=True)
table(doc, [
    ["编号", "验收项", "合格线"],
    ["A1", "手写一个不泄漏内存的 String 类", "能讲清拷贝构造与赋值运算符分别在什么场景被调用"],
    ["A2", "智能指针", "能说明 unique_ptr / shared_ptr / weak_ptr 的适用场景"],
    ["A3", "shared_ptr 循环引用", "能画出循环引用图并给出用 weak_ptr 打破的方案"],
    ["A4", "容器底层", "能说明 vector 扩容策略与迭代器失效的典型场景"],
    ["A5", "提交记录", "GitHub 仓库连续六周有提交，无空窗"],
], widths=[1.6, 6.0, 8.4], align_center=[0])

spacer(doc)

H2(doc, "阶段二 · 并发与第一个项目（W7–W12，2026.12.29 – 2027.02.07）")
P(doc, "寒假是全年最容易被浪费、也最容易出成果的一段时间。", bold=True)
P(doc, "任务清单：", bold=True)
BULLET(doc, "并发基础：std::thread、mutex、lock_guard、condition_variable、atomic、future/promise。")
BULLET(doc, "项目一 · 线程池：任务队列、工作线程、条件变量唤醒、优雅关闭、压测脚本。")
BULLET(doc, "蓝桥杯专题：前缀和与差分、二分、DFS/BFS、背包 DP、并查集。")
BULLET(doc, "蓝桥杯真题：2022–2026 年省赛真题，每周 1–2 套，限时完成。")
P(doc, "验收标准：", bold=True)
table(doc, [
    ["编号", "验收项", "合格线"],
    ["B1", "线程池可运行", "能并发执行任务，能优雅关闭不丢任务、不崩溃"],
    ["B2", "条件变量", "能解释为什么等待要用 while 而不是 if"],
    ["B3", "真题水平", "连续三套真题折算分数稳定在省一区间"],
    ["B4", "项目文档", "有一份 README，写清设计与用法"],
], widths=[1.6, 6.0, 8.4], align_center=[0])

spacer(doc)

H2(doc, "阶段三 · Linux、网络与投递准备（W13–W18，2027.02.08 – 03.21）")
P(doc, "任务清单：", bold=True)
BULLET(doc, "Linux：常用命令、vim、gdb 断点调试、Makefile 与 CMake 编写。")
BULLET(doc, "网络：socket 编程流程、阻塞与非阻塞、select/poll/epoll、Reactor 模型。")
BULLET(doc, "项目二 · 网络库：事件循环、epoll 封装、缓冲区设计、回声服务器与简易 HTTP Server。")
BULLET(doc, "八股第一轮：操作系统与计算机网络（跟着学校课程走，一举两得）。")
BULLET(doc, "简历 v1：一页纸，教育背景、技能、两个项目、蓝桥杯。三月中旬定稿并开始投递。")
P(doc, "验收标准：", bold=True)
table(doc, [
    ["编号", "验收项", "合格线"],
    ["C1", "gdb", "能独立定位一个段错误并打印调用栈"],
    ["C2", "epoll 原理", "能讲清 LT 与 ET 的区别，以及各自的读写时机处理"],
    ["C3", "网络服务", "手写 echo server，能处理多连接"],
    ["C4", "简历", "同学看完能一句话说出你做过什么"],
], widths=[1.6, 6.0, 8.4], align_center=[0])

spacer(doc)

H2(doc, "阶段四 · 投递实战（W19–W24，2027.03.22 – 05.02）")
P(doc, "任务清单：", bold=True)
BULLET(doc, "投递：每周 5–10 家，建立投递台账（公司、岗位、投递日、状态、反馈）。")
BULLET(doc, "面试：每次结束当天写复盘，记录被问倒的问题，48 小时内补齐答案。")
BULLET(doc, "副线：UE5 小 Demo，每周投入约 5 小时，不抢占主线时间。")
BULLET(doc, "4 月中旬蓝桥杯省赛：考前一周只做套题，不学新知识。")
P(doc, "本阶段唯一标准：投递量。三月不投、四月等消息，是最常见的失败方式。", color=ACCENT, bold=True)

spacer(doc)

H2(doc, "阶段五 · 冲刺与入职（W25–W33，2027.05.03 – 07.11）")
P(doc, "任务清单：", bold=True)
BULLET(doc, "八股第二轮：按面经高频顺序过一遍，配合手撕题每天一道。")
BULLET(doc, "项目打磨：补齐压测数据、架构图、README，准备三分钟版本与二十分钟版本的项目介绍。")
BULLET(doc, "降级预案：若成都方向无进展，扩大范围到远程实习、绵阳本地企业与对日外包。")
BULLET(doc, "6 月底前锁定 offer，7 月初入职，并与 HR 确认开学后的出勤安排。")

# ===== 五、项目 =====
H1(doc, "五、项目组合")
P(doc, "简历上只放两个项目，但每一个都必须经得起追问。", bold=True)

H2(doc, "5.1 主线：C++ 高并发服务端三件套")
P(doc, "路线：线程池（1–2 周）→ epoll 网络库（3–4 周）→ HTTP Server（2–3 周）。")
P(doc, "展示的能力：RAII 与智能指针、条件变量、IO 多路复用、缓冲区设计、性能压测与优化。")
table(doc, [
    ["模块", "面试官一定会追问", "你必须能说到的深度"],
    ["线程池", "任务队列怎么加锁？为什么要 while 等条件变量？怎么优雅关闭？",
     "锁的粒度与竞争、虚假唤醒、关闭时如何让任务队列排空"],
    ["网络库", "为什么用 epoll 而不是 select？LT 与 ET 怎么选？惊群怎么处理？",
     "文件描述符上限、事件通知模式、唤醒策略"],
    ["HTTP Server", "QPS 多少？瓶颈在哪？怎么优化的？",
     "压测工具与参数、瓶颈定位（CPU/IO/上下文切换）、优化前后对比数据"],
], widths=[2.2, 7.4, 6.4])

spacer(doc)

H2(doc, "5.2 副线：UE5 小 Demo")
P(doc, "目的不是做出一个完整游戏，而是有一个能玩、且你能讲透的小东西。面试聊到兴趣时，"
       "这比任何教程项目都有说服力。")
P(doc, "如果 UE 的学习成本过高（编译速度、蓝图、打包），果断换 Unity，C# 两周可上手，"
       "不要在一个工具上死磕到三月。", color=ACCENT)

H2(doc, "5.3 唯一判断标准")
callout(doc, "自测方法",
        "对着镜子，连续讲二十分钟你的项目，中间不卡壳、不重复、不出现「这个是我抄的教程」。\n"
        "做不到，就回去重写一遍。")

# ===== 六、蓝桥杯 =====
H1(doc, "六、算法认证：蓝桥杯与 CCF CSP")

H2(doc, "6.1 定位：催化剂，不是目的")
P(doc, "蓝桥杯不是第二条主线，它做两件事：给算法训练加一个截止日期；给简历的「奖项」栏提供一行字。"
       "西南科技大学在 B 组，竞争强度低于 A 组，省一是够得着的目标。")

H2(doc, "6.2 考点与对策")
table(doc, [
    ["考点", "重要度", "对策"],
    ["枚举与模拟", "必考", "限时做历年真题，练手感与细节"],
    ["前缀和 / 差分", "高频", "与面试高频重合，优先练"],
    ["二分", "高频", "整数二分边界、二分答案"],
    ["DFS / BFS", "高频", "网格搜索、状态记录、剪枝"],
    ["动态规划", "高频", "线性 DP、背包、区间 DP 入门"],
    ["图论", "中频", "最短路、并查集"],
    ["数论", "中频", "gcd、快速幂、素数筛"],
    ["字符串", "中频", "KMP、字符串哈希"],
    ["填空题", "必拿", "最稳的分，必须全对"],
    ["暴力骗分", "重要", "不会做的题先写暴力拿部分分"],
], widths=[4.4, 3.0, 8.6], align_center=[0, 1])

spacer(doc)

H2(doc, "6.3 与主线错峰安排")
table(doc, [
    ["时段", "算法任务"],
    ["W1–W6", "Hot 100 基础题型 40 道"],
    ["W7–W12（寒假）", "蓝桥杯专题 + 历年真题，每周 1–2 套"],
    ["W13–W18", "每周 1 套限时套题，保持手感"],
    ["考前一周", "只做套题，不学新知识、不看新题解"],
    ["考后一周", "立刻回到面试高频题，不拖延"],
], widths=[4.6, 11.4])

spacer(doc)
callout(doc, "三条纪律",
        "1. 寒假是危险区。刷题有即时反馈，最容易挤掉项目的整块时间。"
        "项目不完成等于实习无望，蓝桥杯失手只是少一行简历，优先级不能颠倒。\n"
        "2. 不刷“蓝桥杯专用怪题”。近五年省赛真题加基础专题，性价比最高。\n"
        "3. 蓝桥杯的坑多在细节：long long 溢出、多组输入、边界特判——这恰好也是工程能力的一部分。",
        fill="FDECEA")

spacer(doc)

H2(doc, "6.4 CCF CSP 认证（新增 · 2026 年 12 月 13 日）")
P(doc, "CSP 是中国计算机学会主办的软件能力认证，4 小时 5 道编程题，总分 500。"
       "它与蓝桥杯考点高度重叠，但更偏「工程化算法」——题目长、场景多，而且允许带纸质资料。")
table(doc, [
    ["项目", "内容"],
    ["考试时间", "2026 年 12 月 13 日（周日）13:30–17:30"],
    ["报名截止", "个人：12 月 6 日；团体：11 月 24 日（学院统一缴费）"],
    ["费用", "个人 600 元；校内团报 400 元；CCF 会员团报 200 元"],
    ["考试形式", "5 题 × 100 分，4 小时上机"],
    ["特别规则", "可携带纸质资料（书籍、打印材料、手写稿），禁用一切电子设备"],
    ["报名入口", "cspro.org；团体报名识别码 ngV56"],
], widths=[3.0, 13.0])

spacer(doc)
P(doc, "目标设定（本次不冲高分，只拿证书）：", bold=True)
table(doc, [
    ["场次", "目标分数", "策略"],
    ["2026.12.13（本次）", "200 分", "前两题全对即可，后三题战略性放弃"],
    ["2027.09（下一次）", "300 分以上", "补第三题，作为秋招简历的硬指标"],
], widths=[3.6, 3.4, 9.0], align_center=[0, 1])

spacer(doc)
callout(doc, "为什么值得报（官方口径）",
        "1. 委员单位企业（vivo、网易、中兴、思必驰等）对 CSP 考生开通专属内推通道，招聘优先进入下一轮。\n"
        "2. 部分高校将 CSP 成绩作为考研复试、奖学金、保研条件之一。\n"
        "3. 简历「认证」栏的一行字，比空白强得多。\n"
        "4. 可以带纸质资料——考的是「会不会用」，不是「背不背得出」。")

spacer(doc)

H2(doc, "6.5 十二月的 20 天训练表")
table(doc, [
    ["阶段", "日期", "任务", "验收"],
    ["报名", "11.23–11.24", "团体报名交表；错过则 12 月 6 日前个人报名", "报名状态「已提交」"],
    ["摸底", "11.25–11.30", "2018–2020 真题第 1、2 题，每天 2 题", "每题 60 分钟内独立完成"],
    ["提速", "12.01–12.06", "2021–2023 真题 + 整理个人模板册", "每题 45 分钟内完成"],
    ["实战", "12.07–12.12", "2024–2025 整套限时模拟（4 小时）", "稳定 200 分以上"],
    ["考试", "12.13", "上考场", "目标 200+"],
    ["复盘", "12.14–12.31", "期末复习优先，C++ 与算法暂停", "不挂科"],
], widths=[2.0, 3.2, 7.4, 3.4], align_center=[0, 1])

spacer(doc)
P(doc, "模板册清单（可以带纸张，这份册子就是你的外挂）：", bold=True)
BULLET(doc, "字符串处理：读取整行、按空格或逗号分割、进制转换")
BULLET(doc, "日期计算：闰年判断、星期推算、日期差")
BULLET(doc, "大模拟框架：读题 → 建状态 → 逐步推进 → 检查边界")
BULLET(doc, "图与搜索：BFS / DFS 模板、网格方向数组")
BULLET(doc, "前缀和与差分：一维、二维")
BULLET(doc, "二分：整数二分边界、二分答案")
BULLET(doc, "并查集：路径压缩与按秩合并")

spacer(doc)

H2(doc, "6.6 与蓝桥杯的合并策略")
P(doc, "两个考试的考点重叠约七成，不要分开准备，否则时间要翻倍。")
table(doc, [
    ["训练内容", "对蓝桥杯的作用", "对 CSP 的作用"],
    ["历年真题限时训练", "熟悉出题风格", "熟悉题型与节奏"],
    ["基础专题（前缀和、二分、BFS）", "直接对应", "直接对应"],
    ["模板册整理", "考场少写代码", "可带纸质资料，收益更大"],
    ["大模拟题", "较少出现", "CSP 第 1、2 题的主力题型"],
], widths=[4.8, 5.4, 5.8])
P(doc, "一句话：把 CSP 当成蓝桥杯「提前一个月开的模拟赛」。练完再去考蓝桥杯，手感是热的。")

doc.add_page_break()

# ===== 七、投递 =====
H1(doc, "七、投递策略")

H2(doc, "7.1 分层目标")
table(doc, [
    ["层级", "目标", "难度", "投入占比"],
    ["冲刺", "腾讯成都（天美）、网易成都、华为成都研究所、美团成都", "高", "20%"],
    ["主攻", "完美世界成都、西山居、tap4fun、数字天空、极米、迈普", "中", "60%"],
    ["保底", "成都中小软件公司、创业团队、学院合作企业", "低", "20%"],
], widths=[2.2, 10.4, 1.8, 1.6], align_center=[0, 2, 3])

spacer(doc)

H2(doc, "7.2 渠道优先级")
NUM(doc, "内推：学院访企拓岗对接的企业、学长学姐、专业课老师。成功率最高，务必先用。")
NUM(doc, "实习平台：牛客实习板块、BOSS 直聘、实习僧。")
NUM(doc, "公司官网招聘页，尤其是游戏公司的校招与实习专栏。")
NUM(doc, "游戏行业垂直渠道：游戏葡萄、游资网等招聘信息。")

H2(doc, "7.3 投递节奏")
table(doc, [
    ["时间", "动作"],
    ["3 月中旬", "首批 10 家保底层，测试简历反馈率"],
    ["3 月下 – 4 月", "每周 5–10 家，主力是主攻层"],
    ["4 月中旬", "蓝桥杯省赛周，投递暂停"],
    ["5 月", "集中面试，冲刺层开投"],
    ["6 月", "收 offer，协调实习起止时间"],
], widths=[4.2, 11.8])

# ===== 八、时间分配 =====
H1(doc, "八、每周时间分配（基准模型）")
P(doc, "按每周投入 20 小时估算，可根据课业强度等比缩放。")
table(doc, [
    ["板块", "时长", "占比", "说明"],
    ["C++ 与项目", "10 h", "50%", "整块时间，优先级最高"],
    ["算法", "4 h", "20%", "分散到 4–5 天，每天 30 分钟到 1 小时"],
    ["八股（3 月起）", "3 h", "15%", "操作系统与网络，跟着课程走"],
    ["课程与作业", "3 h", "15%", "底线：不挂科"],
], widths=[3.4, 2.6, 2.2, 7.8], align_center=[1, 2])

doc.add_page_break()

# ===== 九、第一周 =====
H1(doc, "九、第一周启动清单（11.18 – 11.24）")
P(doc, "这一周不要求学多少东西，只要求把「基础设施」搭起来。", bold=True)
table(doc, [
    ["☐", "事项", "完成标准"],
    ["☐", "安装开发环境", "VS Code + MinGW-w64 + CMake + Git 均可用，Hello World 能编译运行"],
    ["☐", "建立代码仓库", "GitHub 或 Gitee 建仓，完成首次提交"],
    ["☐", "手写 String 类", "含构造、析构、拷贝构造、赋值运算符，能跑通测试"],
    ["☐", "换掉万能头", "把 bits/stdc++.h 换成标准头文件，养成习惯"],
    ["☐", "打听内推渠道", "问导员或学长，拿到学院访企拓岗对接的成都公司名单"],
    ["☐", "加入备赛群", "算法社团或蓝桥杯备赛群，找到同伴"],
    ["☐", "建立错误本", "新建文档，记录每一道做错或想不出的题"],
], widths=[1.0, 4.6, 10.4], align_center=[0])

# ===== 十、风险 =====
H1(doc, "十、风险与预案")
table(doc, [
    ["风险", "概率", "预案"],
    ["三月投递无回音", "高", "继续投，四月扩到成都中小公司；同步找远程实习"],
    ["寒假项目停滞", "中高", "项目拆成周任务，每周必须有一个可运行的增量"],
    ["蓝桥杯未获省一", "中", "不影响实习主线，省二亦可写入简历"],
    ["期末挂科", "低", "十二月预留两周复习窗口，绝不动用"],
    ["因大二身份被拒", "高", "话术强调可实习时长与连续性；改投对大二友好的中小团队"],
    ["CSP 备考挤占期末复习", "中", "十二月七日起切换优先级：期末高于 CSP；CSP 只求 200 分"],
    ["中途放弃", "中", "每月末复盘一次，只对照验收标准给结论，不给自己讲情绪"],
], widths=[4.2, 2.0, 9.8], align_center=[1])

doc.add_page_break()

# ===== 附录 =====
H1(doc, "附录 A  面试知识清单")

H2(doc, "A.1 C++ 语言（按优先顺序）")
for i, item in enumerate([
    "语言基础：引用与指针的区别、const 的各种用法、static 的三种含义、inline、重载与重写",
    "对象模型：构造与析构顺序、拷贝控制三件套、RAII、虚函数与虚函数表",
    "内存管理：new/delete 与 malloc/free 的区别、智能指针、内存泄漏排查、栈与堆",
    "STL：vector 扩容、map 与 unordered_map 的底层、迭代器失效",
    "并发：thread、mutex、条件变量、原子操作、内存序（了解层面）",
    "现代 C++：auto、lambda、右值引用与完美转发、C++11/14/17 常用特性",
], 1):
    NUML(doc, i, item)

H2(doc, "A.2 操作系统")
for i, item in enumerate([
    "进程与线程的区别、线程的创建与切换开销",
    "进程调度算法与上下文切换",
    "内存管理：分页、虚拟内存、缺页中断",
    "死锁的四个必要条件与避免方法",
    "文件系统基础与 IO 模型（阻塞、非阻塞、多路复用）",
], 1):
    NUML(doc, i, item)

H2(doc, "A.3 计算机网络")
for i, item in enumerate([
    "TCP 三次握手与四次挥手，为什么是三次不是两次",
    "TIME_WAIT 状态存在的原因与时间",
    "TCP 拥塞控制（慢启动、拥塞避免、快重传、快恢复）",
    "HTTP 与 HTTPS 的区别、TLS 握手过程",
    "从浏览器输入 URL 到页面呈现，中间发生了什么",
], 1):
    NUML(doc, i, item)

H2(doc, "A.4 游戏客户端加分项")
for i, item in enumerate([
    "引擎：UE5（C++ 与蓝图）或 Unity（C#），至少能做出一个可玩 Demo",
    "游戏常用数据结构：A* 寻路、四叉树与八叉树空间划分、对象池、状态机",
    "网络同步概念：帧同步与状态同步的区别（了解即可，面试常问）",
    "图形学基础：渲染管线的大致流程（了解即可，不用深挖）",
], 1):
    NUML(doc, i, item)

doc.add_page_break()

H1(doc, "附录 B  学习资源建议")
table(doc, [
    ["方向", "资源类型", "使用方式"],
    ["C++ 语言", "《C++ Primer》第 5 版", "当工具书查，不通读"],
    ["并发编程", "《C++ 并发编程实战》", "配合线程池项目读对应章节"],
    ["网络编程", "《Unix 网络编程》卷 1", "只读 socket 与 IO 多路复用部分"],
    ["算法", "AcWing 算法基础课、洛谷题单", "专题训练 + 历年真题"],
    ["八股", "面经网站与自建笔记", "三月起整理成一问一答形式"],
], widths=[2.6, 6.4, 7.0])

spacer(doc, 12)
hrule(doc)
p = doc.add_paragraph()
p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run("制定于 2026 年 11 月 18 日 · 每周末对照验收标准自查一次")
set_run(r, size=9, color=GRAY)
p.paragraph_format.space_before = Pt(4)

doc.save(OUT)
print("saved:", OUT)
