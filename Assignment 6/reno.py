import numpy as np
import matplotlib.pyplot as plt
from reportlab.platypus import SimpleDocTemplate, Paragraph, Image, Spacer
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.lib.pagesizes import letter
import random

# -------------------------------------------------
# FUNCTION: Run TCP Reno simulation
# -------------------------------------------------
def run_simulation(loss_prob, filename):
    RTT = 0.05          # 50ms RTT
    sim_time = 20       # seconds
    mss = 1.0           # normalized MSS
    initial_cwnd = 1.0
    ssthresh = 64.0

    t = 0.0
    cwnd = initial_cwnd
    time_points = []
    cwnd_trace = []
    bytes_acked = 0

    # Simulated send function
    def send_window(cwnd):
        segs = int(max(1, round(cwnd)))
        lost = False
        acks = 0
        for _ in range(segs):
            if random.random() < loss_prob:
                lost = True
            else:
                acks += 1
        return lost, acks

    # Run simulation (discrete RTT steps)
    while t < sim_time:
        time_points.append(t)
        cwnd_trace.append(cwnd)

        lost, acks = send_window(cwnd)
        bytes_acked += acks

        if lost:   # LOSS → Reno multiplicative decrease
            ssthresh = max(cwnd / 2, 2)
            cwnd = max(cwnd / 2, 1)
        else:
            if cwnd < ssthresh:   # SLOW START
                cwnd *= 2
            else:                 # CONGESTION AVOIDANCE
                cwnd += 1

        t += RTT

    # Throughput
    throughput = bytes_acked / sim_time

    # Plot cwnd graph
    plt.figure(figsize=(10,4))
    plt.step(time_points, cwnd_trace, where='post')
    plt.xlabel("Time (s)")
    plt.ylabel("cwnd (MSS)")
    plt.title(f"TCP Reno – Loss={loss_prob}, Throughput={throughput:.2f} MSS/s")
    plt.grid(True)
    plt.savefig(filename)
    plt.close()

    return throughput


# -------------------------------------------------
# RUN ALL 3 SIMULATION CASES
# -------------------------------------------------
results = []
cases = [
    (0.001, "plot_loss_0_1.png"),  # 0.1% loss
    (0.01,  "plot_loss_1.png"),    # 1% loss
    (0.02,  "plot_loss_2.png")     # 2% loss
]

for loss_prob, fname in cases:
    thr = run_simulation(loss_prob, fname)
    results.append((loss_prob, thr, fname))


# -------------------------------------------------
# CREATE PDF LAB REPORT
# -------------------------------------------------
pdf_path = "TCP_Reno_Lab_Report.pdf"
styles = getSampleStyleSheet()
doc = SimpleDocTemplate(pdf_path, pagesize=letter)

story = []
story.append(Paragraph("TCP Reno Congestion Control Simulation Report", styles['Title']))
story.append(Spacer(1, 12))

intro = """
This report presents a simulation of TCP Reno under three packet loss conditions:
0.1%, 1%, and 2%. Each case includes a congestion window (cwnd) graph and measured
average throughput over a 20-second interval.
"""
story.append(Paragraph(intro, styles['BodyText']))

for loss_prob, thr, fname in results:
    story.append(Paragraph(f"Loss Rate: {loss_prob*100:.1f}%", styles['Heading2']))
    story.append(Paragraph(f"Average Throughput: {thr:.2f} MSS/s", styles['BodyText']))
    story.append(Image(fname, width=400, height=250))
    story.append(Spacer(1, 12))

conclusion = """
As the packet loss rate increases, TCP Reno experiences more frequent multiplicative
decreases in the congestion window (cwnd). At low loss (0.1%), cwnd grows smoothly and
achieves high throughput. At 1% loss, the cwnd oscillations increase and throughput drops.
At 2% loss, cwnd remains small and unstable, producing very low throughput.

Therefore, TCP Reno performs efficiently only in low-loss networks, and increasing packet
loss significantly reduces throughput due to its congestion control behavior.
"""
story.append(Paragraph("Conclusion", styles['Heading2']))
story.append(Paragraph(conclusion, styles['BodyText']))

doc.build(story)

print("PDF generated at:", pdf_path)

