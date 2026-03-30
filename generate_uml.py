import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import FancyBboxPatch
import numpy as np

# ── canvas ────────────────────────────────────────────────────────────────────
W, H = 34, 22
fig, ax = plt.subplots(figsize=(W, H))
ax.set_xlim(0, W)
ax.set_ylim(0, H)
ax.axis("off")
BG = "#1a1a2e"
fig.patch.set_facecolor(BG)
ax.set_facecolor(BG)

# ── palette ───────────────────────────────────────────────────────────────────
C_BOX   = "#252540"
C_CLS   = "#3a7bd5"
C_ENUM  = "#7b3aad"
C_BORD  = "#4a4a6a"
C_ATTR  = "#7ec8a0"
C_METH  = "#f0c050"
C_EVAL  = "#c8a0e0"
C_COMP  = "#e74c3c"
C_ASSOC = "#5dade2"
C_DEP   = "#f39c12"
C_TEXT  = "#dde0f0"

ROW  = 0.38
HDR  = 0.62
PAD  = 0.10

# ─────────────────────────────────────────────────────────────────────────────
def box_height(attrs, meths, is_enum=False):
    sections = len(attrs) + (0 if is_enum else len(meths))
    divs = 0 if is_enum else 1
    return HDR + sections * ROW + divs * 0.06 + PAD * (2 if is_enum else 3)

def draw_box(ax, x, y, w, title, attrs, meths, is_enum=False):
    """Draw class/enum box with top-left at (x, y). Returns (cx_top, cy_top, cx_bot, cy_bot)."""
    h = box_height(attrs, meths, is_enum)
    hcol = C_ENUM if is_enum else C_CLS

    # shadow
    ax.add_patch(FancyBboxPatch((x+0.07, y-h-0.07), w, h,
        boxstyle="round,pad=0.05", lw=0, fc="#00000055", zorder=1))
    # body
    ax.add_patch(FancyBboxPatch((x, y-h), w, h,
        boxstyle="round,pad=0.05", lw=1.2, ec=C_BORD, fc=C_BOX, zorder=2))
    # header band
    ax.add_patch(FancyBboxPatch((x, y-HDR), w, HDR,
        boxstyle="round,pad=0.05", lw=0, fc=hcol, zorder=3))

    # stereotype + name
    stereo = "«enumeration»" if is_enum else "«class»"
    ax.text(x+w/2, y-HDR*0.27, stereo, ha="center", va="center",
            fontsize=6.5, color="#ffffffbb", style="italic", zorder=4)
    ax.text(x+w/2, y-HDR*0.73, title, ha="center", va="center",
            fontsize=9, color="white", fontweight="bold", zorder=4)

    cur = y - HDR - PAD
    if attrs:
        ax.plot([x+0.1, x+w-0.1], [cur+PAD*0.5, cur+PAD*0.5], color=C_BORD, lw=0.7, zorder=4)
        for a in attrs:
            cur -= ROW
            color = C_EVAL if is_enum else C_ATTR
            ax.text(x+0.18, cur+ROW*0.5, a, ha="left", va="center",
                    fontsize=7.2, color=color, fontfamily="monospace", zorder=4)
        cur -= PAD

    if not is_enum and meths:
        ax.plot([x+0.1, x+w-0.1], [cur+PAD*0.5, cur+PAD*0.5], color=C_BORD, lw=0.7, zorder=4)
        for m in meths:
            cur -= ROW
            ax.text(x+0.18, cur+ROW*0.5, m, ha="left", va="center",
                    fontsize=7.2, color=C_METH, fontfamily="monospace", zorder=4)

    # connection ports
    cx = x + w/2
    return cx, y, cx, y-h   # top-centre, bottom-centre

def midpoint_port(x, y, w, attrs, meths, is_enum=False):
    h = box_height(attrs, meths, is_enum)
    return x, y-h/2, x+w, y-h/2   # left-mid x,y  right-mid x,y

# ─────────────────────────────────────────────────────────────────────────────
# Arrow helpers
def arr(ax, x1,y1, x2,y2, col, lw=1.5, ls="-", label="", rad=0.0):
    ax.annotate("", xy=(x2,y2), xytext=(x1,y1),
        arrowprops=dict(arrowstyle="-|>", color=col, lw=lw,
                        linestyle=ls,
                        connectionstyle=f"arc3,rad={rad}"), zorder=6)
    if label:
        mx,my = (x1+x2)/2, (y1+y2)/2
        ax.text(mx+0.12, my, label, fontsize=6, color=col, zorder=7,
                bbox=dict(fc=BG, ec="none", pad=0.5))

def comp_arr(ax, x1,y1, x2,y2, col=C_COMP, label="", rad=0.0):
    """Filled diamond + arrow."""
    dx,dy = x2-x1, y2-y1
    L = np.hypot(dx,dy)
    ux,uy = dx/L, dy/L
    px,py = -uy, ux
    d, dlen = 0.14, 0.32
    pts = [(x1,y1),
           (x1+ux*dlen/2+px*d, y1+uy*dlen/2+py*d),
           (x1+ux*dlen,        y1+uy*dlen),
           (x1+ux*dlen/2-px*d, y1+uy*dlen/2-py*d)]
    ax.add_patch(plt.Polygon(pts, closed=True, fc=col, ec=col, zorder=6))
    ax.annotate("", xy=(x2,y2), xytext=(x1+ux*dlen, y1+uy*dlen),
        arrowprops=dict(arrowstyle="-|>", color=col, lw=1.5,
                        connectionstyle=f"arc3,rad={rad}"), zorder=6)
    if label:
        mx,my = (x1+x2)/2, (y1+y2)/2
        ax.text(mx+0.12, my, label, fontsize=6, color=col, zorder=7,
                bbox=dict(fc=BG, ec="none", pad=0.5))

# ─────────────────────────────────────────────────────────────────────────────
# Layout — place classes
# Grid:
#   Row A (top):    enums
#   Row B (upper):  Car  Lane  Road  TrafficLight
#   Row C (middle): LightController
#   Row D (bottom): Intersection

# ── Enums (row A, y=21.5) ─────────────────────────────────────────────────
E_Y = 21.5
draw_box(ax, 0.4,  E_Y, 4.0, "Direction",
         ["NORTH","SOUTH","EAST","WEST"], [], is_enum=True)

draw_box(ax, 5.4,  E_Y, 4.0, "LightState",
         ["RED","GREEN","YELLOW"], [], is_enum=True)

draw_box(ax, 10.6, E_Y, 4.6, "Phase",
         ["NS_GREEN","NS_YELLOW","EW_GREEN","EW_YELLOW"], [], is_enum=True)

# ── Classes (row B, y=16.5) ───────────────────────────────────────────────
B_Y = 16.4

car_attrs  = ["-id : int", "-speed : int", "-colour : string"]
car_meths  = ["+Car(id, speed, colour)",
              "+getId() : int",
              "+getSpeed() : int",
              "+getColor() : string"]
draw_box(ax, 0.4, B_Y, 5.5, "Car", car_attrs, car_meths)

lane_attrs = ["-id : int", "-QueueLane : bool",
              "-cars : queue<Car>", "-road : Road*"]
lane_meths = ["+Lane(id, road, QueueLane)",
              "+addCar(car) : void",
              "+removeCar() : Car",
              "+isEmpty() : bool",
              "+getId() : int",
              "+isQueueLane() : bool"]
draw_box(ax, 7.0, B_Y, 6.2, "Lane", lane_attrs, lane_meths)

road_attrs = ["-id : int", "-direction : Direction",
              "-queueLane : Lane*", "-arrivalLane : Lane*"]
road_meths = ["+Road(id, direction, qL, aL)",
              "+getQueueLane() : Lane*",
              "+getArrivalLane() : Lane*",
              "+getDirection() : Direction",
              "+getId() : int"]
draw_box(ax, 14.4, B_Y, 6.6, "Road", road_attrs, road_meths)

tl_attrs  = ["-id : int", "-lightState : LightState", "-road : Road*"]
tl_meths  = ["+TrafficLight(id, lightState, road)",
             "+getId() : int",
             "+getState() : LightState",
             "+getRoad() : Road*",
             "-setState(newState) : void"]
draw_box(ax, 22.2, B_Y, 7.2, "TrafficLight", tl_attrs, tl_meths)

# ── LightController (row C, y=9.5) ────────────────────────────────────────
C_Y = 9.6

lc_attrs = ["-trafficLights : map<Direction, TrafficLight*>",
            "-currentPhase : Phase",
            "-timer : int"]
lc_meths = ["+LightController(trafficLights)",
            "+updateLights(deltaTime : int) : void",
            "+isGreen(direction : Direction) : bool",
            "-setLights(ns : LightState, ew : LightState) : void"]
draw_box(ax, 11.5, C_Y, 10.0, "LightController", lc_attrs, lc_meths)

# ── Intersection (row D, y=3.8) ────────────────────────────────────────────
D_Y = 4.0

int_attrs = ["-id : int",
             "-roads : map<Direction, Road*>",
             "-lightController : LightController*"]
int_meths = ["+Intersection(id, roads, lightController)",
             "+getId() : int",
             "+update(deltaTime : int) : void",
             "+addCar(direction, car) : void",
             "+getRoad(direction) : Road*",
             "+getLightController() : LightController*",
             "-moveCars(from, to : Direction) : void"]
draw_box(ax, 7.2, D_Y, 12.0, "Intersection", int_attrs, int_meths)

# ─────────────────────────────────────────────────────────────────────────────
# Arrows
# ── Lane ◆── Car  (Lane owns queue of Car)
comp_arr(ax, 7.0, 13.5, 5.9, 13.5, label="contains 0..*")

# ── Road ◆── Lane  (Road owns 2 lanes)
comp_arr(ax, 14.4, 13.2, 13.2, 13.2, label="owns 2")

# ── Lane --> Road  (back-pointer, dashed assoc)
arr(ax, 10.1, 10.2, 14.4, 11.5, C_ASSOC, ls="dashed", label="belongs to", rad=0.15)

# ── TrafficLight --> Road
arr(ax, 22.2, 13.0, 21.0, 13.0, C_ASSOC, label="associated with")

# ── TrafficLight ..uses.. LightState
arr(ax, 25.8, 16.4, 7.4, 20.6, C_ASSOC, ls="dashed", label="uses")

# ── Road uses Direction
arr(ax, 17.7, 16.4, 2.4, 20.6, C_ASSOC, ls="dashed", label="uses")

# ── LightController ◆── TrafficLight (composition, 4 lights)
comp_arr(ax, 21.5, 8.0, 22.2, 12.2, label="manages 4")

# ── LightController «friend» TrafficLight  (dashed, orange)
arr(ax, 20.5, 7.0, 25.0, 12.2, C_DEP, ls="dashed", label="«friend»\naccesses setState()", rad=-0.2)

# ── LightController uses Phase
arr(ax, 16.5, 9.6, 12.9, 20.6, C_ASSOC, ls="dashed", label="uses", rad=0.1)

# ── LightController uses Direction
arr(ax, 14.0, 9.6, 2.4, 20.6, C_ASSOC, ls="dashed", label="uses", rad=0.05)

# ── LightController uses LightState
arr(ax, 15.5, 9.6, 7.4, 20.6, C_ASSOC, ls="dashed", label="uses", rad=0.08)

# ── Intersection ◆── Road (4 roads)
comp_arr(ax, 12.5, 1.7, 17.7, 12.8, label="owns 4", rad=-0.1)

# ── Intersection ◆── LightController
comp_arr(ax, 13.2, 4.0, 16.5, 6.4, label="owns 1")

# ── Intersection uses Direction
arr(ax, 8.5, 1.7, 2.4, 20.0, C_ASSOC, ls="dashed", label="uses", rad=0.1)

# ─────────────────────────────────────────────────────────────────────────────
# Legend
lx, ly = 25.5, 5.2
ax.add_patch(FancyBboxPatch((lx-0.3, ly-3.2), 8.2, 3.8,
    boxstyle="round,pad=0.1", lw=1, ec=C_BORD, fc=C_BOX, zorder=2))
ax.text(lx+3.5, ly+0.35, "Legend", ha="center", fontsize=9,
        color=C_TEXT, fontweight="bold", zorder=5)
items = [
    (C_COMP,  "◆──▶  Composition"),
    (C_ASSOC, "──▶   Association / Uses"),
    (C_ASSOC, "- -▶  «uses» (dashed)"),
    (C_DEP,   "- -▶  «friend» dependency"),
    (C_CLS,   "■     Class"),
    (C_ENUM,  "■     Enumeration"),
]
for i,(col,txt) in enumerate(items):
    ax.text(lx, ly-0.1-i*0.50, txt, fontsize=7.5, color=col, zorder=5)

# Title
ax.text(W/2, H-0.35, "Traffic Management System — UML Class Diagram",
        ha="center", va="center", fontsize=14.5,
        color=C_TEXT, fontweight="bold")

plt.tight_layout(pad=0.2)
plt.savefig("UML.png", dpi=200, bbox_inches="tight",
            facecolor=fig.get_facecolor())
print("Saved UML.png")
